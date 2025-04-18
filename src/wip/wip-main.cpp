#include <chrono>
#include <engine/Engine.hpp>
#include <engine/log.hpp>
#include <regex>
#include <utility>
#include <yaml-cpp/yaml.h>
#include "engine/Map.hpp"
#include "engine/components/graphics.hpp"
#include "engine/components/world.hpp"

void print(const char* msg, engine::ServiceRegistry&) { LOG_DEBUG(msg); }

void init(engine::ServiceRegistry& reg);


void engine_main(engine::ServiceRegistry& reg) {
  const std::shared_ptr lifecycle = reg.get_service<engine::EngineLifecycle>();

  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::init_post, std::bind_front(print, "init_post"));
  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::exit_pre, std::bind_front(print, "exit_pre"));
  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::exit_post, std::bind_front(print, "exit_post"));

  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::init_post, init);
}

using namespace engine::components;

void init(engine::ServiceRegistry& reg) {
  const std::shared_ptr scheduler = reg.get_service<engine::SystemScheduler>();

  scheduler->add_system("helloworlder",[last_update = std::chrono::steady_clock::now()](entt::registry& ecs) mutable {
    const auto now = std::chrono::steady_clock::now();
    if (now - last_update < std::chrono::seconds(1)) {
      return;
    }
    last_update = now;
    // LOG_INFO("HI DUDDLES");
  });

  const std::shared_ptr map = reg.get_service<engine::Map>();
  const std::shared_ptr registry = reg.get_service<entt::registry>();


  const engine::Instance& instance = map->create_instance("megainstance");
  for (int x = -5; x <= 5; ++x) {
    for (int y = -5; y <= 5; ++y) {
      const entt::entity entity = registry->create();
      registry->emplace<world::Instance>(entity, instance.id);
      registry->emplace<world::Rectangle>(entity, 1, 1);
      registry->emplace<world::Position>(entity, x, y);
      registry->emplace<graphics::Renderable>(entity);
      registry->emplace<graphics::Color>(entity, 1, 0, 1);
    }
  }
}
