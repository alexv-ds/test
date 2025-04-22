#include "Engine.hpp"
#include "MapImpl.hpp"
#include "SokolStatus.hpp"
#include "systems/InputWASDPositionController.hpp"
#include "systems/RenderSokolBegin.hpp"
#include "systems/RenderSokolDraw.hpp"
#include "systems/RenderSokolEnd.hpp"
#include "systems/SyncCameraWithMainWindow.hpp"
#include "systems/UpdateBBox.hpp"
#include "systems/UpdateMap.hpp"

namespace engine {

  Engine::Engine() {
    this->ecs_registry = std::make_shared<entt::registry>();
    this->service_registry = std::make_unique<ServiceRegistry>();
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
    {

      if (!sokol_status->is_gfx_initialized() ||
          !sokol_status->is_util_imgui_initialized() ||
          !sokol_status->is_sokol_initialized() || !sokol_status->is_spg_initialized()) {
        throw std::runtime_error("sokol is not fully initialized");
      }
      sokol_status->set_imgui_events_interception(true);
      system_scheduler->add_system("RenderSokolBegin",
                                   std::make_shared<systems::RenderSokolBegin>());
      system_scheduler->add_system("RenderSokolDraw",
                                   std::make_shared<systems::RenderSokolDraw>(map));
      system_scheduler->add_system("RenderSokolEnd",
                                   std::make_shared<systems::RenderSokolEnd>());
    }

    if (sokol_status->is_sokol_initialized()) {
      system_scheduler->add_system("InputWASDPositionController",
                                   std::make_shared<systems::InputWASDPositionController>(
                                     this->service_registry->get_service<Input>()));

      system_scheduler->add_system("SyncCameraWithMainWindow",
                                   std::make_shared<systems::SyncCameraWithMainWindow>());
    }
  }

  void Engine::run() {
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::frame_pre);
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::system_run_pre);
    this->scheduler().run_systems();
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::system_run_post);
    lifecycle().emit(*this->service_registry, EngineLifecycle::Stage::frame_post);
  }
} // namespace engine
