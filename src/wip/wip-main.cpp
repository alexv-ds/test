#include <chrono>
#include <engine/Engine.hpp>
#include <engine/Map.hpp>
#include <engine/components/graphics.hpp>
#include <engine/components/other.hpp>
#include <engine/components/world.hpp>
#include <engine/hsv_rgb_conversion.hpp>
#include <engine/log.hpp>
#include <random>
#include <sokol_app.h>

#include "engine/Input.hpp"

void init(engine::ServiceRegistry& locator);

void engine_main(engine::ServiceRegistry& reg) {
  const std::shared_ptr lifecycle = reg.get_service<engine::EngineLifecycle>();
  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::init_post, init);
}

using namespace engine::components;

void init(engine::ServiceRegistry& locator) {
  const std::shared_ptr scheduler = locator.get_service<engine::SystemScheduler>();
  const std::shared_ptr registry = locator.get_service<entt::registry>();
  const world::Instance instance{
    locator.get_service<engine::Map>()->create_instance("world").id};

  const auto camera = registry->create();
  registry->emplace<world::Instance>(camera, instance.id);
  registry->emplace<world::Rectangle>(camera, 4.0, 4.0);
  registry->emplace<world::Position>(camera);
  registry->emplace<graphics::Invisible>(camera);
  registry->emplace<other::InputWASDPositionController>(
    camera, other::InputWASDPositionController{.speed = 3, .shift_speed = 10});
  registry->emplace<other::InputQEScaleController>(camera);
  registry->emplace<graphics::Camera>(camera);
  registry->emplace<graphics::CameraLinkWithMainWindow>(
    camera, graphics::CameraLinkWithMainWindow{.preferred_height = 14});
  registry->emplace<world::Scale>(camera);

  for (int x = -20; x <= 20; x += 2) {
    for (int y = -20; y <= 20; y += 2) {
      const auto e = registry->create();
      registry->emplace<world::Instance>(e, instance.id);
      registry->emplace<world::Rectangle>(e, 1, 1);
      registry->emplace<world::Position>(e, x, y);
    }
  }
}
