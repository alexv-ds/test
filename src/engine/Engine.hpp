#pragma once
#include <entt/entity/registry.hpp>
#include "ServiceRegistry.hpp"
#include "EngineLifecycle.hpp"
#include "SystemScheduler.hpp"

namespace engine
{
  class Engine
  {
  public:
    Engine();

    [[nodiscard]] EngineLifecycle &lifecycle() const
    {
      return *engine_lifecycle;
    }
    [[nodiscard]] SystemScheduler& scheduler() const
    {
      return *system_scheduler;
    }

    void init();
    void run();

  public:
    std::shared_ptr<entt::registry> ecs_registry;
    std::unique_ptr<ServiceRegistry> service_registry;
    std::shared_ptr<EngineLifecycle> engine_lifecycle;
    std::shared_ptr<SystemScheduler> system_scheduler;
  };
}
