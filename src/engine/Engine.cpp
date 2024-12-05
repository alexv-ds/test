#include "Engine.hpp"


namespace engine
{

  Engine::Engine()
  {
    this->ecs_registry = std::make_shared<entt::registry>();
    this->service_registry = std::make_shared<ServiceRegistry>();
    this->engine_lifecycle = std::make_shared<service::EngineLifecycle>();
    service_registry->add_service(this->engine_lifecycle);

    this->system_scheduler = std::make_shared<SystemScheduler>("config/systems.yml");
  }

  void Engine::run()
  {
    lifecycle().emit(*this, service::EngineLifecycle::Stage::frame_pre);
    lifecycle().emit(*this, service::EngineLifecycle::Stage::system_run_pre);
    lifecycle().emit(*this, service::EngineLifecycle::Stage::system_run_post);
    lifecycle().emit(*this, service::EngineLifecycle::Stage::frame_post);
  }
}
