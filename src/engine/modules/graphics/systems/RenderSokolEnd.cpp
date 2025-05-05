#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
#include <sokol_gp.h>
#include <util/sokol_imgui.h>

#include "RenderSokolEnd.hpp"

namespace engine::graphics::systems {
  void RenderSokolEnd::update() {
    sg_pass pass = {};
    pass.swapchain = sglue_swapchain();
    sg_begin_pass(&pass);
    // Dispatch all draw commands to Sokol GFX.
    sgp_flush();
    // Finish a draw command queue, clearing it.
    sgp_end();
    // // Draw imgui
    simgui_render();
    // End render pass.
    sg_end_pass();
    // Commit Sokol render.
    sg_commit();
  }
} // namespace engine::graphics::systems
