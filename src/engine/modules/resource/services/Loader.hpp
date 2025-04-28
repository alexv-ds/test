#pragma once
#include <engine/EngineLifecycle.hpp>
#include <engine/helpers/TimeMeter.hpp>
#include <engine/noncopyable.hpp>
#include <expected>
#include <memory>
#include <sokol_fetch.h>
#include <span>

namespace engine::resource {
  class Resource : noncopyable {
  public:
    using DataType = std::expected<std::span<const std::uint8_t>, std::string>;

    ~Resource();

    [[nodiscard]] bool finished() const noexcept { return this->finished_; }
    void cancel();
    DataType data() noexcept;

  private:
    friend class Loader;
    constexpr static std::uint32_t chunk_size = 1048576;

    void on_response(const sfetch_response_t*);

    std::string file_;
    std::vector<std::uint8_t> data_;
    sfetch_handle_t fetch_handle_{.id = 0};
    bool finished_ = false;
    std::string error;

    helpers::TimeMeter timer_;

    explicit Resource(std::string&& file);
  };

  class Loader {
  public:
    explicit Loader(std::shared_ptr<EngineLifecycle> engine_lifecycle);
    ~Loader();

    Resource load(std::string&& file) { return Resource{std::move(file)}; }

  private:
    EngineLifecycle::CallbackId cb_id_;
    std::shared_ptr<EngineLifecycle> engine_lifecycle_;

    void update();
  };
} // namespace engine::resource
