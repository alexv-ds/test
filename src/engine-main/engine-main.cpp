#include <engine/Engine.hpp>
#include <engine/log.hpp>
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
#include <sokol_gp.h>
#include <util/sokol_imgui.h>
#include <spdlog/cfg/env.h>
#include "InputImpl.hpp"
#include "SokolStatusImpl.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

void engine_main(engine::ServiceRegistry&);

std::unique_ptr<engine::Engine> g_engine;
std::shared_ptr<InputImpl> g_input;
static bool g_imgui_event_interception_flag = false;

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

  // Sokol GFX
  {
    sg_desc sg_desc = {};
    sg_desc.environment = sglue_environment();
    sg_setup(&sg_desc);
    if (!sg_isvalid()) {
      LOG_CRITICAL("Failed to create Sokol GFX context");
      std::exit(EXIT_FAILURE);
    }
  }


  // SGP
  {
    constexpr sgp_desc sgp_desc = {};
    sgp_setup(&sgp_desc);
    if (!sgp_is_valid()) {
      LOG_CRITICAL("Failed to create Sokol GP context: {}",
                   sgp_get_error_message(sgp_get_last_error()));
      std::exit(EXIT_FAILURE);
    }
  }


  // Imgui
  {
    constexpr simgui_desc_t simgui_desc = {};
    simgui_setup(&simgui_desc);
  }

  // Engine
  try {
    g_engine = std::make_unique<engine::Engine>();
    g_engine->service_registry->add_service<engine::SokolStatus>(
      std::make_shared<SokolStatusImpl>(&g_imgui_event_interception_flag));
    g_input = std::make_unique<InputImpl>();
    g_engine->service_registry->add_service<engine::Input>(g_input);
    g_engine->init();
    engine_main(*g_engine->service_registry);
  }
  catch (const std::exception& e) {
    LOG_CRITICAL("Engine initialization error: {}", e.what());
    std::rethrow_exception(std::current_exception());
  }

  g_engine->lifecycle().emit(*g_engine->service_registry,
                             engine::EngineLifecycle::Stage::init_pre);
  g_engine->lifecycle().emit(*g_engine->service_registry,
                             engine::EngineLifecycle::Stage::init_post);
}

void cleanup() {
  // engine
  if (g_engine) {
    g_engine->lifecycle().emit(*g_engine->service_registry,
                               engine::EngineLifecycle::Stage::exit_pre);
    g_engine->lifecycle().emit(*g_engine->service_registry,
                               engine::EngineLifecycle::Stage::exit_post);
  }
  g_engine = nullptr;
  g_input = nullptr;

  // Imgui
  simgui_shutdown();

  // SGP
  sgp_shutdown();

  // Sokol GFX
  sg_shutdown();
}

void frame() {
  g_engine->run();
}

void event(const sapp_event* ev) {
  if (!g_input) {
    LOG_CRITICAL("Cannot handle window event: input instance not created");
    std::exit(EXIT_FAILURE);
  }

  if (g_imgui_event_interception_flag && simgui_handle_event(ev)) {
    if (ev->type == SAPP_EVENTTYPE_KEY_UP || ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
      g_input->keyboard_clean();
    }
    return;
  }

  try {
    if (ev->type == SAPP_EVENTTYPE_KEY_UP || ev->type == SAPP_EVENTTYPE_KEY_DOWN) {
      g_input->keyboard_handle_event(ev);
    }
  }
  catch (const std::exception& e) {
    LOG_ERROR("Window event handle error: {}", e.what());
  }
}

sapp_desc sokol_main(int, char**) {
  sapp_desc desc = {};
  desc.init_cb = init;
  desc.frame_cb = frame;
  desc.cleanup_cb = cleanup;
  desc.event_cb = event;
  desc.window_title = "Test";
  desc.high_dpi = true;
  desc.win32_console_utf8 = true;
  desc.win32_console_create = false;
  desc.win32_console_attach = true;
  desc.icon.sokol_default = true;
  desc.sample_count = 8;
  desc.html5_canvas_name = "wasm-app";
  desc.html5_canvas_resize = false;

  return desc;
}
