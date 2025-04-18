#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
#include <sokol_gp.h>

#include <chrono>
#include <cmath>
#include <entt/entt.hpp>
#include <print>
#include <random>

#ifdef _WIN32
#include <windows.h>
#endif

struct rgb {
  float r; // a fraction between 0 and 1
  float g; // a fraction between 0 and 1
  float b; // a fraction between 0 and 1
};

struct hsv {
  float h; // angle in degrees
  float s; // a fraction between 0 and 1
  float v; // a fraction between 0 and 1
};

rgb hsv2rgb(hsv in) {
  double hh, p, q, t, ff;
  long i;
  rgb out;

  if (in.s <= 0.0f) { // < is bogus, just shuts up warnings
    out.r = in.v;
    out.g = in.v;
    out.b = in.v;
    return out;
  }
  hh = in.h;
  if (hh >= 360.0)
    hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = in.v * (1.0 - in.s);
  q = in.v * (1.0 - (in.s * ff));
  t = in.v * (1.0 - (in.s * (1.0 - ff)));

  switch (i) {
  case 0:
    out.r = in.v;
    out.g = t;
    out.b = p;
    break;
  case 1:
    out.r = q;
    out.g = in.v;
    out.b = p;
    break;
  case 2:
    out.r = p;
    out.g = in.v;
    out.b = t;
    break;

  case 3:
    out.r = p;
    out.g = q;
    out.b = in.v;
    break;
  case 4:
    out.r = t;
    out.g = p;
    out.b = in.v;
    break;
  case 5:
  default:
    out.r = in.v;
    out.g = p;
    out.b = q;
    break;
  }
  return out;
}

struct Quad {
  float x;
  float y;
  float r;
  float g;
  float b;
};

Quad random_quad(const float x, const float y) {
  thread_local std::default_random_engine random_engine{std::random_device{}()};
  std::uniform_real_distribution dist_hue{0.0f, 360.0f};
  auto [r, g, b] = hsv2rgb({.h = dist_hue(random_engine), .s = 1.0f, .v = 1.0f});
  return {
    .x = x,
    .y = y,
    .r = r,
    .g = g,
    .b = b,
  };
}

std::vector<Quad> g_quads;

void init() {
#ifdef _WIN32
  if (const void* const handle = sapp_win32_get_hwnd()) {
    const auto window = static_cast<HWND>(const_cast<void*>(handle));
    ShowWindow(window, SW_MAXIMIZE);
  }
#endif
  std::println("HI DUDDLES");

  sg_desc sgdesc = {};
  sgdesc.environment = sglue_environment();
  sg_setup(&sgdesc);
  if (!sg_isvalid()) {
    fprintf(stderr, "Failed to create Sokol GFX context!\n");
    exit(-1);
  }
  sgp_desc sgpdesc = {};
  sgp_setup(&sgpdesc);
  if (!sgp_is_valid()) {
    fprintf(stderr, "Failed to create Sokol GP context: %s\n",
            sgp_get_error_message(sgp_get_last_error()));
    exit(-1);
  }

  // g_quads.push_back(random_quad(0, 0));

  for (int x = -4; x <= 4; x++) {
    for (int y = -4; y <= 4; y++) {
      g_quads.emplace_back(random_quad(x, y));
    }
  }
}


void frame() {


  // Get current window size.
  int width = sapp_width(), height = sapp_height();
  // Begin recording draw commands for a frame buffer of size (width, height).
  sgp_begin(width, height);
  // Set frame buffer drawing region to (0,0,width,height).
  sgp_viewport(0, 0, width, height);
  // Set drawing coordinate space to (left=-ratio, right=ratio, top=1, bottom=-1).


  const float ratio = width / (float)height;
  const float cells_y = 9;
  const float cells_x = cells_y * ratio;

   
  sgp_project(-cells_x * 0.5f, cells_x * 0.5f, cells_y * 0.5f, -cells_y * 0.5f);

  // Clear the frame buffer.
  sgp_set_color(0.1f, 0.1f, 0.1f, 1.0f);
  sgp_clear();

  // Draw an animated rectangle that rotates and changes its colors.


  const float time = std::chrono::duration_cast<std::chrono::duration<float>>(
                       std::chrono::steady_clock::now().time_since_epoch())
                       .count();

  // sgp_rotate_at(time, 0.0f, 0.0f);
  const float size = std::lerp(0.3f, 1.0f, std::abs(std::sin(time / 3)));

  for (const auto& quad : g_quads) {
    const float half_size = size * 0.5f;
    sgp_set_color(quad.r, quad.g, quad.b, 1.0f);
    sgp_draw_filled_rect(quad.x - half_size, quad.y - half_size, size, size);
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

void cleanup() {
  sgp_shutdown();
  sg_shutdown();
}

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
  return desc;
}
