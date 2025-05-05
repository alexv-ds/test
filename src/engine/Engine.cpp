#include "Engine.hpp"
#include "MapImpl.hpp"
#include "ModuleLoader.hpp"
#include "ModuleRegistry.hpp"
#include "ModuleRegistryImpl.hpp"
#include "SokolStatus.hpp"
#include "systems/InputQEScaleController.hpp"
#include "systems/InputWASDPositionController.hpp"
#include "systems/UpdateBBox.hpp"
#include "systems/UpdateMap.hpp"
#include "systems/simple_systems.hpp"

#include "modules/graphics.hpp"
#include "modules/resource.hpp"

namespace engine {

  Engine::Engine() {
    this->ecs_registry = std::make_shared<entt::registry>();
    this->service_registry = std::make_shared<ServiceRegistry>();
    this->engine_lifecycle = std::make_shared<EngineLifecycle>();
    this->system_scheduler =
      std::make_shared<SystemScheduler>("config/systems.yml", this->ecs_registry);

    service_registry->add_service(this->ecs_registry);
    service_registry->add_service(this->engine_lifecycle);
    service_registry->add_service(this->system_scheduler);
  }

  void Engine::init() {
    const auto map = std::make_shared<MapImpl>();
    // Map
    {
      service_registry->add_service<Map>(map);
      system_scheduler->add_system("UpdateBBox", std::make_shared<systems::UpdateBBox>());
      system_scheduler->add_system("UpdateMap",
                                   std::make_shared<systems::UpdateMap>(map));
    }

    const auto sokol_status = service_registry->get_service<SokolStatus>();
    if (sokol_status->is_sokol_initialized()) {
      system_scheduler->add_system("InputWASDPositionController",
                                   std::make_shared<systems::InputWASDPositionController>(
                                     this->service_registry->get_service<Input>()));


      system_scheduler->add_system(
        "InputQEScaleController",
        std::bind_back(systems::InputQEScaleController,
                       this->service_registry->get_service<Input>()));
    }

    systems::add_simple_systems(*system_scheduler);

    const auto module_registry =
      std::make_shared<ModuleRegistryImpl>(this->service_registry);
    this->service_registry->add_service<ModuleLoader>(module_registry);
    this->service_registry->add_service<ModuleRegistry>(module_registry);

    module_registry->define(resource::module_define);
    module_registry->define(graphics::module_define);
  }

  void Engine::run() {
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::frame_pre);
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::system_run_pre);
    this->scheduler().run_systems();
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::system_run_post);
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::frame_post);
  }
} // namespace engine
