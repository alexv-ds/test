#include <engine/Engine.hpp>
#include <engine/log.hpp>
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
#include <sokol_gp.h>
#include <spdlog/cfg/env.h>
#include <engine/SokolStatus.hpp>

#ifdef _WIN32
#include <windows.h>
#endif

class SokolStatusImpl final : public engine::SokolStatus {
public:
  bool is_sokol_initialized() const noexcept override { return true; }
  bool is_gfx_initialized() const noexcept override { return true; }
  bool is_spg_initialized() const noexcept override { return true; }
};

void engine_main(engine::ServiceRegistry&);

std::unique_ptr<engine::Engine> g_engine;

void init() {
#ifdef SPDLOG_ACTIVE_LEVEL
  spdlog::set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
#endif
  spdlog::cfg::load_env_levels();

#ifdef _WIN32
  if (const void* const handle = sapp_win32_get_hwnd()) {
    ShowWindow(static_cast<HWND>(const_cast<void*>(handle)), SW_MAXIMIZE);
  }
  else {
    LOG_WARN("Cannot maximize windows: sapp_win32_get_hwnd() returns nullptr");
  }
#endif

  sg_desc sg_desc = {};
  sg_desc.environment = sglue_environment();
  sg_setup(&sg_desc);
  if (!sg_isvalid()) {
    LOG_CRITICAL("Failed to create Sokol GFX context");
    std::exit(EXIT_FAILURE);
  }

  constexpr sgp_desc sgp_desc = {};
  sgp_setup(&sgp_desc);
  if (!sgp_is_valid()) {
    LOG_CRITICAL("Failed to create Sokol GP context: {}",
                 sgp_get_error_message(sgp_get_last_error()));
    std::exit(EXIT_FAILURE);
  }

  try {
    g_engine = std::make_unique<engine::Engine>();
    g_engine->service_registry->add_service<engine::SokolStatus>(
      std::make_shared<SokolStatusImpl>());
    g_engine->init();
    engine_main(*g_engine->service_registry);
  }
  catch (const std::exception& e) {
    LOG_CRITICAL("Exception: {}", e.what());
    std::rethrow_exception(std::current_exception());
  }

  g_engine->lifecycle().emit(*g_engine->service_registry,
                             engine::EngineLifecycle::Stage::init_pre);
  g_engine->lifecycle().emit(*g_engine->service_registry,
                             engine::EngineLifecycle::Stage::init_post);
}

void cleanup() {
  if (g_engine) {
    g_engine->lifecycle().emit(*g_engine->service_registry,
                               engine::EngineLifecycle::Stage::exit_pre);
    g_engine->lifecycle().emit(*g_engine->service_registry,
                               engine::EngineLifecycle::Stage::exit_post);
  }
  g_engine = nullptr;
  sgp_shutdown();
  sg_shutdown();
}

void frame() { g_engine->run(); }

sapp_desc sokol_main(int, char**) {
  sapp_desc desc = {};
  desc.init_cb = init;
  desc.frame_cb = frame;
  desc.cleanup_cb = cleanup;
  desc.window_title = "Test";
  desc.high_dpi = true;
  desc.win32_console_utf8 = true;
  desc.win32_console_create = false;
  desc.win32_console_attach = true;
  desc.icon.sokol_default = true;
  desc.sample_count = 8;
  return desc;
}
