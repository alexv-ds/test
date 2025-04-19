#include <chrono>
#include <engine/Engine.hpp>
#include <engine/log.hpp>
#include <regex>
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
#include <sokol_gp.h>
#include <utility>
#include <yaml-cpp/yaml.h>
#include "engine/Map.hpp"
#include "engine/SokolStatus.hpp"
#include "engine/components/graphics.hpp"
#include "engine/components/world.hpp"

void init(engine::ServiceRegistry& locator);

void engine_main(engine::ServiceRegistry& reg) {
  const std::shared_ptr lifecycle = reg.get_service<engine::EngineLifecycle>();
  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::init_post, init);
}

using namespace engine::components;


void SokolBegin(entt::registry&) {
  const int width = sapp_width();
  const int height = sapp_height();
  sgp_begin(width, height);
  sgp_viewport(0, 0, width, height);

  // const float ratio = width / static_cast<float>(height);
  // const float cells_y = 12;
  // const float cells_x = cells_y * ratio;
  // sgp_project(-cells_x * 0.5f, cells_x * 0.5f, cells_y * 0.5f, -cells_y * 0.5f);
  sgp_set_color(0.1f, 0.1f, 0.1f, 1.0f);
  sgp_clear();
}

void SokolEnd(entt::registry&) {
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

void SokolDraw(entt::registry& reg) {
  const float ratio = sapp_widthf() / sapp_heightf();
  const float cells_y = 12;
  const float cells_x = cells_y * ratio;
  sgp_project(-cells_x * 0.5f, cells_x * 0.5f, cells_y * 0.5f, -cells_y * 0.5f);
  for (const auto [e, pos, rect] :
       reg.view<world::Position, world::Rectangle>(entt::exclude<graphics::Invisible>)
         .each()) {
    world::Rotation rotation{.0};
    world::Scale scale{0, 0};
    graphics::Color color{1, 1, 1};
    if (const auto* p_rotation = reg.try_get<const world::Rotation>(e)) {
      rotation = *p_rotation;
    }
    if (const auto* p_scale = reg.try_get<const world::Scale>(e)) {
      scale = *p_scale;
    }
    if (const auto* p_color = reg.try_get<const graphics::Color>(e)) {
      color = *p_color;
    }



    sgp_push_transform();
    sgp_translate(pos.x - (rect.width * scale.x) * 0.5f, pos.y - (rect.height * scale.x) * 0.5f);
    sgp_scale(scale.x, scale.y);
    sgp_rotate(rotation.theta);
    sgp_set_color(color.r, color.g, color.b, 1.0f);
    sgp_draw_filled_rect(0, 0, rect.width, rect.height);
    sgp_pop_transform();


    //////////////////////////////////////
    // sgp_scale(scale.x, scale.y);
    // sgp_rotate(rotation.theta);
    // sgp_draw_filled_rect(pos.x - (rect.width * scale.x) * 0.5f,
    //                      pos.y - (rect.height * scale.x) * 0.5f, rect.width * scale.x,
    //                      rect.height * scale.y);
    // sgp_draw_filled_rect(pos.x - rect.width * 0.5f, pos.y - rect.height * 0.5f,
    //                      rect.width, rect.height);
  }
}

void init(engine::ServiceRegistry& locator) {
  const std::shared_ptr scheduler = locator.get_service<engine::SystemScheduler>();
  const std::shared_ptr sokol_status = locator.get_service<engine::SokolStatus>();
  if (!sokol_status->is_sokol_initialized() || !sokol_status->is_gfx_initialized() ||
      !sokol_status->is_spg_initialized()) {
    throw std::runtime_error("Sokol is not fully (base, gfx, sgp) initialized");
  }
  scheduler->add_system("SokolBegin", SokolBegin);
  scheduler->add_system("SokolEnd", SokolEnd);
  scheduler->add_system("SokolDraw", SokolDraw);

  const std::shared_ptr map = locator.get_service<engine::Map>();
  const std::shared_ptr registry = locator.get_service<entt::registry>();

  const engine::Instance& instance = map->create_instance("megainstance");
  for (int x = -5; x <= 5; ++x) {
    for (int y = -5; y <= 5; ++y) {
      const entt::entity entity = registry->create();
      registry->emplace<world::Instance>(entity, instance.id);
      registry->emplace<world::Rectangle>(entity, 1, 1);
      registry->emplace<world::Position>(entity, x, y);
      registry->emplace<world::Scale>(entity, 0.9, 0.9);
      registry->emplace<graphics::Color>(entity, 1, 1, 0);
      registry->emplace<world::Rotation>(entity, 0);
    }
  }

  scheduler->add_system("scaler", [](entt::registry& reg) {
    for (const auto [e, scale] : reg.view<world::Scale>().each()) {
      const float time = std::chrono::duration_cast<std::chrono::duration<float>>(
                           std::chrono::steady_clock::now().time_since_epoch())
                           .count();
      const float size = std::lerp(0.3f, 1.0f, std::abs(std::sin(time / 3)));
      scale.x = size;
      scale.y = size;
      reg.patch<world::Scale>(e);
    }
  });


  scheduler->add_system("rotator", [](entt::registry& reg) {
    for (const auto [e, rotate] : reg.view<world::Rotation>().each()) {
      const float time = std::chrono::duration_cast<std::chrono::duration<float>>(
                           std::chrono::steady_clock::now().time_since_epoch())
                           .count();
      const float angle = std::sin(time / 3);
      rotate.theta = angle;
      reg.patch<world::Rotation>(e);
    }
  });
}
