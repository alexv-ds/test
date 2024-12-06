#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_gp.h>
#include <sokol_glue.h>

#include <entt/entt.hpp>
#include <print>
#include <chrono>
#include <cmath>

void init()
{
  std::println("HI DUDDLES");

  sg_desc sgdesc = {};
  sgdesc.environment = sglue_environment();
  sg_setup(&sgdesc);
  if (!sg_isvalid())
  {
    fprintf(stderr, "Failed to create Sokol GFX context!\n");
    exit(-1);
  }
  sgp_desc sgpdesc = {};
  sgp_setup(&sgpdesc);
  if (!sgp_is_valid())
  {
    fprintf(stderr, "Failed to create Sokol GP context: %s\n", sgp_get_error_message(sgp_get_last_error()));
    exit(-1);
  }
}


struct Quad
{
  float x;
  float y;
  float r;
  float g;
  float b;
};

void frame()
{
  // Get current window size.
  int width = sapp_width(), height = sapp_height();
  float ratio = width / (float)height;

  // float height = 10;

  // Begin recording draw commands for a frame buffer of size (width, height).
  sgp_begin(width, height);
  // Set frame buffer drawing region to (0,0,width,height).
  sgp_viewport(0, 0, width, height);
  // Set drawing coordinate space to (left=-ratio, right=ratio, top=1, bottom=-1).
  sgp_project(-5, 5, 5, -5);
  // sgp_project(0, 0, 10.0f, 10.0f);

  // Clear the frame buffer.
  sgp_set_color(0.1f, 0.1f, 0.1f, 1.0f);
  sgp_clear();

  // Draw an animated rectangle that rotates and changes its colors.


  const float time = std::chrono::duration_cast<std::chrono::duration<float>>(
    std::chrono::steady_clock::now().time_since_epoch()).count();

  sgp_set_color(1.0f, 1.0f, 0.3f, 1.0f);
  // sgp_rotate_at(time, 0.0f, 0.0f);

  std::vector<Quad> quads = []()
  {
    std::vector<Quad> q;
    // for (int x = -5; x < 5; ++x)
    // {
    //   for (int y = -5; y < 5; ++y)
    //   {
    //     Quad quad = {
    //       .x = (float)x,
    //       .y = (float)y,
    //       .r = 1.0f,
    //       .g = 1.0f,
    //       .b = 1.0f,
    //     };
    //     q.push_back(quad);
    //   }
    // }
    Quad quad = {
      .x = 1,
      .y = 1,
      .r = 1,
      .g = 0,
      .b = 0,
    };
    q.push_back(quad);
    return q;
  }();


  const float size = std::lerp(0.3f,5.0f,std::abs(std::sin(time / 3)));

  for (const auto& quad : quads)
  {
    const float half_size = size * 0.5f;
    sgp_draw_filled_rect(quad.x - half_size , quad.y - half_size, size, size);
  }

  // Begin a render pass.
  sg_pass pass = {};
  pass.swapchain = sglue_swapchain();
  sg_begin_pass(&pass);
  // Dispatch all draw commands to Sokol GFX.
  sgp_flush();
  // Finish a draw command queue, clearing it.
  sgp_end();
  // End render pass.
  sg_end_pass();
  // Commit Sokol render.
  sg_commit();

}

void cleanup()
{
  sgp_shutdown();
  sg_shutdown();
}

sapp_desc sokol_main(int, char **)
{
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
