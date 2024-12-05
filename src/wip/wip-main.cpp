// #include <sokol_app.h>
// #include <sokol_gfx.h>
// #include <sokol_gp.h>
// #include <sokol_glue.h>
// #include <entt/entt.hpp>
// #include <print>
//
// void init();
// void frame();
// void cleanup();
//
// sapp_desc sokol_main(int, char **)
// {
//   sapp_desc desc = {};
//   desc.init_cb = init;
//   desc.frame_cb = frame;
//   desc.cleanup_cb = cleanup;
//   desc.window_title = "Test";
//   desc.high_dpi = true;
//   desc.win32_console_utf8 = true;
//   desc.win32_console_create = false;
//   desc.win32_console_attach = true;
//   desc.icon.sokol_default = true;
//   return desc;
// }
//
// // ///////////////////////////////////////////////////////////
//
// struct Position
// {
//   float x = 0;
//   float y = 0;
// };
//
// struct Quad
// {
//   float width = 1;
//   float height = 1;
// };
//
// struct Color
// {
//   float r = 1;
//   float g = 1;
//   float b = 1;
// };
//
// struct Opacity
// {
//   float alpha = 1; // [0,1]
// };
//
// // ///////////////////////////////////////////////////////////
//
// entt::registry g_registry;
//
// void cleanup()
// {
//   sgp_shutdown();
//   sg_shutdown();
// }
//
// void init()
// {
//   sg_desc sgdesc = {};
//   sgdesc.environment = sglue_environment();
//   sg_setup(&sgdesc);
//   if (!sg_isvalid())
//   {
//     fprintf(stderr, "Failed to create Sokol GFX context!\n");
//     exit(-1);
//   }
//   sgp_desc sgpdesc = {};
//   sgp_setup(&sgpdesc);
//   if (!sgp_is_valid())
//   {
//     fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
//     exit(-1);
//   }
//
//   for (int x = 0; x < 10; ++x)
//   {
//     for (int y = 0; y < 10; ++y)
//     {
//       auto entity = g_registry.create();
//       g_registry.emplace<Position>(entity, static_cast<float>(x), static_cast<float>(y));
//       g_registry.emplace<Quad>(entity, 1.0f, 1.0f);
//       g_registry.emplace<Opacity>(entity, 1.0f);
//       g_registry.emplace<Color>(entity, 1.0f, 0.0f, 1.0f);
//     }
//   }
// }
//
// void frame()
// {
//   const int window_width = sapp_width();
//   const int window_height = sapp_height();
//   // float ratio = static_cast<float>(window_width) / static_cast<float>(window_height);
//   sgp_begin(window_width, window_height);
//   sgp_viewport(0, 0, window_width, window_height);
//   sgp_project(0, 10, 10, 0);
//   sgp_set_color(0.2f, 0.2f, 0.2f, 1.0f);
//   sgp_clear();
//
//   // //////////////////////////////////////////
//
//   g_registry.view<const Position, const Quad>().each([](entt::entity e, const Position &pos, const Quad &quad)
//   {
//     Color color = {};
//     if (const auto* p_color = g_registry.try_get<const Color>(e))
//     {
//       color = *p_color;
//     }
//
//     Opacity opacity = {};
//     if (const auto* p_opacity = g_registry.try_get<const Opacity>(e))
//     {
//       opacity = *p_opacity;
//     }
//
//     sgp_set_color(color.r, color.g, color.b, opacity.alpha);
//
//     const float half_width = quad.width * 0.5f;
//     const float half_height = quad.height * 0.5f;
//     sgp_draw_filled_rect(pos.x - half_width , pos.y - half_height, quad.width, quad.height);
//   });
//
//
//   // //////////////////////////////////////////
//   sg_pass pass = {};
//   pass.swapchain = sglue_swapchain();
//   sg_begin_pass(&pass);
//   sgp_flush();
//   sgp_end();
//   sg_end_pass();
//   sg_commit();
// }

#include <engine/Engine.hpp>
#include <engine/log.hpp>



void print(const char* msg, engine::Engine&)
{
  ENGINE_DEBUG(msg);
}

void engine_main(engine::Engine& engine)
{
  engine.lifecycle().add_callback_static(engine::service::EngineLifecycle::Stage::init_post, std::bind_front(print, "init_post"));
  // engine.lifecycle().add_callback_static(engine::service::EngineLifecycle::Stage::frame_pre, std::bind_front(print, "frame_pre"));
  // engine.lifecycle().add_callback_static(engine::service::EngineLifecycle::Stage::frame_post, std::bind_front(print, "frame_post"));
  engine.lifecycle().add_callback_static(engine::service::EngineLifecycle::Stage::system_run_pre, std::bind_front(print, "system_run_pre"));
  // engine.lifecycle().add_callback_static(engine::service::EngineLifecycle::Stage::system_run_post, std::bind_front(print, "system_run_post"));
  engine.lifecycle().add_callback_static(engine::service::EngineLifecycle::Stage::exit_pre, std::bind_front(print, "exit_pre"));
  engine.lifecycle().add_callback_static(engine::service::EngineLifecycle::Stage::exit_post, std::bind_front(print, "exit_post"));
}