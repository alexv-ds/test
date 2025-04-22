#include "RenderSokolBegin.hpp"
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_gp.h>
#include <util/sokol_imgui.h>

namespace engine::systems {

  void RenderSokolBegin::update() {
    const int width = sapp_width();
    const int height = sapp_height();
    sgp_begin(width, height);
    sgp_viewport(0, 0, width, height);
    sgp_set_color(0.1f, 0.1f, 0.1f, 1.0f);
    sgp_clear();

    const simgui_frame_desc_t simgui_frame_desc = {.width = width,
                                                   .height = height,
                                                   .delta_time = sapp_frame_duration(),
                                                   .dpi_scale = sapp_dpi_scale()};
    simgui_new_frame(&simgui_frame_desc);
  }

} // namespace engine::systems
