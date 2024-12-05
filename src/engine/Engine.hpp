#pragma once
#include <entt/entity/registry.hpp>
#include "ServiceRegistry.hpp"
#include "service/EngineLifecycle.hpp"
#include "SystemScheduler.hpp"

namespace engine
{
  class Engine
  {
  public:
    Engine();

    [[nodiscard]] service::EngineLifecycle &lifecycle() const
    {
      return *engine_lifecycle;
    }
    [[nodiscard]] SystemScheduler& scheduler() const
    {
      return *system_scheduler;
    }


    void run();

  private:
    std::shared_ptr<entt::registry> ecs_registry;
    std::shared_ptr<ServiceRegistry> service_registry;
    std::shared_ptr<service::EngineLifecycle> engine_lifecycle;
    std::shared_ptr<SystemScheduler> system_scheduler;
  };
}
