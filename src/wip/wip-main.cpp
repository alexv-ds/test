
#include <chrono>
#include <engine/Engine.hpp>
#include <engine/Map.hpp>
#include <engine/components/other.hpp>
#include <engine/components/world.hpp>
#include <engine/log.hpp>
#include <engine/modules/resource/services/Loader.hpp>
#include <sokol_app.h>
#include <engine/ModuleLoader.hpp>
#include <engine/modules/graphics.hpp>
#include <engine/modules/graphics/components.hpp>

void init(engine::ServiceRegistry& locator);

void engine_main(engine::ServiceRegistry& reg) {
  const std::shared_ptr lifecycle = reg.get_service<engine::EngineLifecycle>();
  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::init_post, init);
}

using namespace engine::components;
namespace graphics = engine::graphics::components;


void ImguiTest(entt::registry&);

std::vector<std::uint8_t> g_sound;

void init(engine::ServiceRegistry& locator) {
  const auto module_loader = locator.get_service<engine::ModuleLoader>();
  module_loader->load(engine::graphics::module_name);

  const std::shared_ptr scheduler = locator.get_service<engine::SystemScheduler>();
  const std::shared_ptr registry = locator.get_service<entt::registry>();
  const world::Instance instance{
    locator.get_service<engine::Map>()->create_instance("world").id};


  const auto i_am = registry->create();
  registry->emplace<world::Instance>(i_am, instance.id);
  registry->emplace<world::Rectangle>(i_am, 1, 1);
  registry->emplace<world::Position>(i_am);
  registry->emplace<graphics::Color>(i_am, 1, 0, 0);
  registry->emplace<other::InputWASDPositionController>(
    i_am, other::InputWASDPositionController{.speed = 3, .shift_speed = 10});
  registry->emplace<graphics::Layer>(i_am, 10);
  // registry->emplace<graphics::BlendMode>(i_am, graphics::BlendMode::add);
  // registry->emplace<graphics::Transparency>(i_am, 0.5);

  const auto camera = registry->create();
  registry->emplace<world::Instance>(camera, instance.id);
  registry->emplace<world::Rectangle>(camera);
  registry->emplace<world::Position>(camera);
  registry->emplace<graphics::Invisible>(camera);
  registry->emplace<other::InputQEScaleController>(camera);
  registry->emplace<graphics::Camera>(camera);
  registry->emplace<graphics::CameraLinkWithMainWindow>(
    camera, graphics::CameraLinkWithMainWindow{.preferred_height = 14});
  registry->emplace<world::Scale>(camera);
  registry->emplace<world::CopyPositionFrom>(camera, i_am);


  for (int x = -20; x <= 20; x += 2) {
    for (int y = -20; y <= 20; y += 2) {
      const auto e = registry->create();
      registry->emplace<world::Instance>(e, instance.id);
      registry->emplace<world::Rectangle>(e, 1, 1);
      registry->emplace<world::Position>(e, x, y);
      registry->emplace<graphics::Sprite>(e, "resources/floor.png");
      // registry->emplace<graphics::Color>(e, 0,1,0);
    }
  }

}


