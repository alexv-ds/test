#include "Loader.hpp"
#include <atomic>
#include <cstring>
#include <engine/helpers/sokol_log.hpp>
#include <sokol_fetch.h>

#include "engine/EngineLifecycle.hpp"

namespace engine::resource {
  // //////////////////////////// //
  // ///////// Resource ///////// //
  // //////////////////////////// //

  Resource::Resource(std::string&& file, Callback&& cb) :
      file_(std::move(file)), cb_(std::move(cb)) {
    LOG_TRACE("Resource '{}' - loading started", this->file_);
    this->data_.resize(chunk_size);
    const void* user_data = this;
    const sfetch_request_t req{
      .channel = 0,
      .path = this->file_.c_str(),
      .callback =
        [](const sfetch_response_t* res) {
          Resource* self = nullptr;
          std::memcpy(&self, res->user_data, sizeof(void*));
          self->on_response(res);
        },
      .chunk_size = chunk_size,
      .buffer = {.ptr = this->data_.data(), .size = this->data_.size()},
      .user_data = {.ptr = &user_data, .size = sizeof(void*)}};
    this->fetch_handle_ = sfetch_send(req);
  }

  void Resource::execute_callback() const noexcept {
    if (this->cb_) {
      try {
        this->cb_(*this);
      }
      catch (const std::exception& e) {
        LOG_ERROR("Resouce callback execution error: {}", e.what());
      }
    }
  }

  Resource::~Resource() {
    if (sfetch_handle_valid(this->fetch_handle_)) {
      LOG_DEBUG("Resource fetch canceled - {}", this->file_);
      sfetch_cancel(this->fetch_handle_);
    }
  }

  void Resource::cancel() {
    if (sfetch_handle_valid(this->fetch_handle_)) {
      sfetch_cancel(this->fetch_handle_);
    }
  }

  Resource::DataType Resource::data() const noexcept {
    if (!this->error.empty()) {
      return std::unexpected(this->error);
    }
    return this->data_;
  }

  void Resource::on_response(const sfetch_response_t* res) {
    if (!this->error.empty()) {
      return;
    }
    if (res->failed) {
      this->data_.clear();
      const char* const errmsg = [code = res->error_code] {
        switch (code) {
        case SFETCH_ERROR_NO_ERROR:
          return "SFETCH_ERROR_NO_ERROR";
        case SFETCH_ERROR_FILE_NOT_FOUND:
          return "SFETCH_ERROR_FILE_NOT_FOUND";
        case SFETCH_ERROR_NO_BUFFER:
          return "SFETCH_ERROR_NO_BUFFER";
        case SFETCH_ERROR_BUFFER_TOO_SMALL:
          return "SFETCH_ERROR_BUFFER_TOO_SMALL";
        case SFETCH_ERROR_UNEXPECTED_EOF:
          return "SFETCH_ERROR_UNEXPECTED_EOF";
        case SFETCH_ERROR_INVALID_HTTP_STATUS:
          return "SFETCH_ERROR_INVALID_HTTP_STATUS";
        case SFETCH_ERROR_CANCELLED:
          return "SFETCH_ERROR_CANCELLED";
        default:
          return "UNKNOWN ERROR";
        }
      }();
      this->error = errmsg;
      LOG_ERROR("Resource '{}' loading error: {}", this->file_, this->error);
      this->execute_callback();
      return;
    }
    if (res->cancelled) {
      this->data_.clear();
      this->error = "CANCELLED";
      LOG_TRACE("Resource loading canceled '{}'", this->file_);
      this->execute_callback();
      return;
    }
    if (res->paused) {
      return;
    }

    sfetch_unbind_buffer(res->handle);
    this->data_.resize(this->data_.size() - (chunk_size - res->data.size) +
                       (res->finished ? 0 : chunk_size));
    sfetch_bind_buffer(res->handle,
                       {.ptr = &this->data_.back() - chunk_size, .size = chunk_size});

    if (res->finished) {
      this->finished_ = true;
      LOG_INFO("Resource loaded '{}' - {:.2f}s", this->file_,
               this->timer_.measure<std::chrono::duration<float>>().count());
      this->execute_callback();
    }
  }

  // //////////////////////////// //
  // ////////// Loader ////////// //
  // //////////////////////////// //

  static std::atomic_bool g_fetch_initialized = false;

  Loader::Loader(std::shared_ptr<EngineLifecycle> engine_lifecycle) :
      engine_lifecycle_(std::move(engine_lifecycle)) {
    if (g_fetch_initialized) {
      throw std::runtime_error("Loader already initialized");
    }

    sfetch_setup({
      .max_requests = 128,
      .num_channels = 5,
      .num_lanes = 10,
      .allocator = {},
      .logger = {.func = helpers::sokol_log, .user_data = nullptr},
    });

    g_fetch_initialized = true;

    this->cb_id_ = this->engine_lifecycle_->add_callback(
      EngineLifecycle::Stage::system_run_pre, [this](auto&) { this->update(); });
  }

  Loader::~Loader() {
    this->engine_lifecycle_->remove_callback(this->cb_id_);
    sfetch_shutdown();
    g_fetch_initialized = false;
  }

  void Loader::update() { sfetch_dowork(); }

} // namespace engine::resource
