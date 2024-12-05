#include <sokol_app.h>
#include <engine/Engine.hpp>
#include <spdlog/cfg/env.h>
#include <engine/log.hpp>


void engine_main(engine::Engine &engine);

std::unique_ptr<engine::Engine> g_engine;

void init()
{
#ifdef SPDLOG_ACTIVE_LEVEL
  spdlog::set_level(static_cast<spdlog::level::level_enum>(SPDLOG_ACTIVE_LEVEL));
#endif
  spdlog::cfg::load_env_levels();
  g_engine->lifecycle().emit(*g_engine, engine::service::EngineLifecycle::Stage::init_post);
}

void cleanup()
{
  g_engine->lifecycle().emit(*g_engine, engine::service::EngineLifecycle::Stage::exit_pre);
  g_engine->lifecycle().emit(*g_engine, engine::service::EngineLifecycle::Stage::exit_post);
}

void frame()
{
  g_engine->run();
}

sapp_desc sokol_main(int, char **)
{
  g_engine = std::make_unique<engine::Engine>();
  engine_main(*g_engine);
  g_engine->lifecycle().emit(*g_engine, engine::service::EngineLifecycle::Stage::init_pre);

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
  return desc;
}
