#include "ResourceLoader.hpp"
#include <atomic>
#include <engine/log.hpp>
#include <sokol_fetch.h>

namespace engine::resource::systems {

  static std::atomic_bool g_fetch_initialized{false};

  static void sokol_log(
    const char* tag, // always "sfetch"
    uint32_t log_level, // 0=panic, 1=error, 2=warning, 3=info
    uint32_t /*log_item_id*/, // SFETCH_LOGITEM_*
    const char* message_or_null, // a message string, may be nullptr in release mode
    uint32_t line_nr, // line number in sokol_fetch.h
    const char* filename_or_null, // source filename, may be nullptr in release mode
    void* /*user_data*/) {

    if (!message_or_null) {
      return;
    }

    const char* const safe_tag = tag ? tag : "unknown";

    const spdlog::level::level_enum level = [log_level] {
      switch (log_level) {
      case 0:
        return spdlog::level::critical;
      case 1:
        return spdlog::level::err;
      case 3:
        return spdlog::level::info;
      default:
        return spdlog::level::warn;
      }
    }();

    const spdlog::source_loc loc{filename_or_null, static_cast<int>(line_nr), nullptr};
    spdlog::default_logger()->log(loc, level, "[{}] {}", safe_tag, message_or_null);


  }

  ResourceLoader::ResourceLoader() {
    if (g_fetch_initialized) {
      throw std::runtime_error("sokol_fetch already initialized");
    }
    constexpr sfetch_desc_t desc{
      .max_requests = 128,
      .num_channels = 5,
      .num_lanes = 5,
      .allocator = {},
      .logger = {.func = sokol_log, .user_data = nullptr},
    };
    sfetch_setup(desc);
    g_fetch_initialized = true;
  }

  ResourceLoader::~ResourceLoader() {
    sfetch_shutdown();
    g_fetch_initialized = false;
  }

  void ResourceLoader::start() {
    static uint8_t buf[200'000];

    sfetch_request_t req{};
    req.path = "resources/do-not-play-me.mp3";
    req.buffer = {
      .ptr = buf,
      .size = sizeof(buf),
    };
    req.callback = [](const sfetch_response_t* response) {
      if (response->failed) {
        LOG_ERROR("FETCH ERROR: {}", (int)response->error_code);
      }
      LOG_INFO("MEGACALLBACK");
    };


    sfetch_send(req);
  }

  void ResourceLoader::stop() {}

  void ResourceLoader::update() { sfetch_dowork(); }

} // namespace engine::resource::systems
