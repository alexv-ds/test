#include <chrono>
#include <engine/Engine.hpp>
#include <engine/Input.hpp>
#include <engine/Map.hpp>
#include <engine/SokolStatus.hpp>
#include <engine/components/graphics.hpp>
#include <engine/components/world.hpp>
#include <engine/hsv_rgb_conversion.hpp>
#include <engine/log.hpp>
#include <random>
#include <regex>
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
#include <sokol_gp.h>
#include <utility>

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
    world::Scale scale{1, 1};
    graphics::Color color{1, 1, 1};
    graphics::Transparency transparency{1};
    if (const auto* p_rotation = reg.try_get<const world::Rotation>(e)) {
      rotation = *p_rotation;
    }
    if (const auto* p_scale = reg.try_get<const world::Scale>(e)) {
      scale = *p_scale;
    }
    if (const auto* p_color = reg.try_get<const graphics::Color>(e)) {
      color = *p_color;
    }
    if (const auto* p_transparency = reg.try_get<const graphics::Transparency>(e)) {
      transparency.a = std::clamp(p_transparency->a, 0.0f, 1.0f);
    }

    sgp_set_color(color.r, color.g, color.b, transparency.a);
    sgp_set_blend_mode(SGP_BLENDMODE_BLEND);

    sgp_push_transform();
    sgp_translate(pos.x - rect.width * scale.x * 0.5f,
                  pos.y - rect.height * scale.x * 0.5f);
    sgp_scale(scale.x, scale.y);
    sgp_rotate_at(rotation.theta, rect.width * 0.5f, rect.height * 0.5f);
    sgp_draw_filled_rect(0.f, 0.f, rect.width, rect.height);
    sgp_pop_transform();

    sgp_reset_blend_mode();
    sgp_reset_color();

    if (const auto* bbox = reg.try_get<world::BoundingBox>(e)) {
      sgp_set_color(1.f, 1.f, 1.f, 1.0f);
      sgp_set_blend_mode(SGP_BLENDMODE_BLEND);
      sgp_push_transform();
      sgp_translate(pos.x - bbox->width * 0.5f, pos.y - bbox->height * 0.5f);
      sgp_draw_filled_rect(0.f, 0.f, bbox->width, bbox->height);
      sgp_pop_transform();
      sgp_reset_blend_mode();
      sgp_reset_color();
    }
  }
}

struct BBoxDrawer {
  entt::entity target = entt::null;
};

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
  for (int x = -5; x <= 5; x += 2) {
    for (int y = -5; y <= 5; y += 2) {
      const entt::entity entity = registry->create();
      registry->emplace<world::Instance>(entity, instance.id);
      registry->emplace<world::Rectangle>(entity, 1, 1);
      registry->emplace<world::Position>(entity, x, y);
      registry->emplace<graphics::Color>(entity, 1, 1, 1);
      // registry->emplace<graphics::Transparency>(entity, .7f);
      // registry->emplace<world::Scale>(entity);
      registry->emplace<world::Rotation>(entity);
    }
  }

  scheduler->add_system("scaler", [](entt::registry& reg) {
    for (const auto [e, scale] : reg.view<world::Scale>().each()) {
      const float time = std::chrono::duration_cast<std::chrono::duration<float>>(
                           std::chrono::steady_clock::now().time_since_epoch())
                           .count();
      const float size = std::lerp(0.3f, 1.0f, std::abs(std::sin(time / 4)));
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
      constexpr float angular_speed = 0.2 * 3.1415926;
      rotate.theta = angular_speed * time;
      reg.patch<world::Rotation>(e);
    }
  });

  {
    std::shared_ptr input = locator.get_service<engine::Input>();
    const auto i_am = registry->create();
    registry->emplace<world::Instance>(i_am, instance.id);
    registry->emplace<world::Rectangle>(i_am, 0.7, 0.7);
    registry->emplace<world::Position>(i_am);
    registry->emplace<world::Rotation>(i_am);
    registry->emplace<graphics::Color>(i_am, 1, 1, 0);
    scheduler->add_system("iam-mover", [i_am, input](entt::registry& reg) {
      using KeyCode = engine::Input::KeyCode;
      auto& position = reg.get_or_emplace<world::Position>(i_am);
      constexpr float step_size = 0.05;
      bool pos_updated = false;
      if (input->keyboard(KeyCode::w)) {
        position.y += step_size;
        pos_updated = true;
      }
      if (input->keyboard(KeyCode::s)) {
        position.y -= step_size;
        pos_updated = true;
      }
      if (input->keyboard(KeyCode::a)) {
        position.x -= step_size;
        pos_updated = true;
      }
      if (input->keyboard(KeyCode::d)) {
        position.x += step_size;
        pos_updated = true;
      }
      if (pos_updated) {
        reg.patch<world::Position>(i_am);
      }
    });

    scheduler->add_system("intersect_colorizer", [i_am, map](entt::registry& reg) {
      const world::Instance* instance = reg.try_get<world::Instance>(i_am);
      const world::Position* position = reg.try_get<world::Position>(i_am);
      const world::BoundingBox* bbox = reg.try_get<world::BoundingBox>(i_am);
      if (!instance || !position || !bbox) {
        return;
      }
      const engine::Instance* map_instance = map->try_get_instance(instance->id);
      if (!map_instance) {
        return;
      }
      std::vector<entt::entity> entities;
      map_instance->query(entities, *position, *bbox);
      std::default_random_engine random_engine{std::random_device{}()};
      std::uniform_real_distribution<float> hue_dist(0.0, 360.0);
      for (const auto e : entities) {
        if (e == i_am) {
          continue;
        }
        const engine::Rgb rgb = engine::hsv2rgb({hue_dist(random_engine), 1, 1});
        reg.emplace_or_replace<graphics::Color>(e, rgb.r, rgb.g, rgb.b);
      }
    });
  }
}
