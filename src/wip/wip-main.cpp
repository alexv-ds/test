#include <chrono>
#include <engine/Engine.hpp>
#include <engine/Input.hpp>
#include <engine/Map.hpp>
#include <engine/SokolStatus.hpp>
#include <engine/components/graphics.hpp>
#include <engine/components/other.hpp>
#include <engine/components/world.hpp>
#include <engine/hsv_rgb_conversion.hpp>
#include <engine/log.hpp>
// #include <imgui.h>
#include <imgui.h>
#include <random>
#include <regex>
#include <sokol_app.h>
#include <sokol_gfx.h>
#include <sokol_glue.h>
#include <sokol_gp.h>
#include <util/sokol_imgui.h>
#include <utility>

void init(engine::ServiceRegistry& locator);

void engine_main(engine::ServiceRegistry& reg) {
  const std::shared_ptr lifecycle = reg.get_service<engine::EngineLifecycle>();
  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::init_post, init);
}

using namespace engine::components;

struct BBoxDrawer {
  entt::entity target = entt::null;
};

void init(engine::ServiceRegistry& locator) {
  const std::shared_ptr scheduler = locator.get_service<engine::SystemScheduler>();
  const std::shared_ptr map = locator.get_service<engine::Map>();
  const std::shared_ptr registry = locator.get_service<entt::registry>();

  const engine::Instance& instance = map->create_instance("megainstance");
  for (int x = -5; x <= 500; x += 2) {
    for (int y = -5; y <= 500; y += 2) {
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


  const auto i_am = registry->create();
  registry->emplace<world::Instance>(i_am, instance.id);
  registry->emplace<world::Rectangle>(i_am, 4.0, 4.0);
  registry->emplace<world::Position>(i_am);
  registry->emplace<graphics::Invisible>(i_am);
  registry->emplace<other::InputWASDPositionController>(
    i_am, other::InputWASDPositionController{.speed = 3, .shift_speed = 10});
  registry->emplace<graphics::Camera>(i_am);
  registry->emplace<graphics::CameraLinkWithMainWindow>(
    i_am, graphics::CameraLinkWithMainWindow{.preferred_height = 15});
}
