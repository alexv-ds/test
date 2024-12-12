#include "Engine.hpp"


namespace engine
{

  Engine::Engine()
  {
    this->ecs_registry = std::make_shared<entt::registry>();
    this->service_registry = std::make_shared<ServiceRegistry>();
    this->engine_lifecycle = std::make_shared<EngineLifecycle>();
    this->system_scheduler = std::make_shared<SystemScheduler>("config/systems.yml", this->ecs_registry);

    service_registry->add_service(this->engine_lifecycle);
    service_registry->add_service(this->system_scheduler);
  }

  void Engine::run()
  {
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::frame_pre);
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::system_run_pre);
    this->scheduler().run_systems();
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::system_run_post);
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::frame_post);
  }
}
