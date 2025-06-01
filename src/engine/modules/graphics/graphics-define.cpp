#include <engine/SokolStatus.hpp>
#include <engine/modules/graphics.hpp>
#include <engine/modules/resource.hpp>

#include "engine/SystemScheduler.hpp"
#include "systems/RenderSokolBegin.hpp"
#include "systems/RenderSokolDraw.hpp"
#include "systems/RenderSokolEnd.hpp"
#include "systems/SyncCameraWithMainWindow.hpp"
#include "systems/TextureLoader.hpp"


namespace engine::graphics {

  void init(ServiceRegistry&);

  const ModuleDefine module_define = ModuleDefineStandalone{
    .name = module_name,
    .require = {resource::module_name},
    .optional = {},
    .init = init,
  };

  void init(ServiceRegistry& locator) {
    const auto sokol_status = locator.get_service<SokolStatus>();
    const auto scheduler = locator.get_service<SystemScheduler>();
    const auto map = locator.get_service<Map>();

    if (!sokol_status->is_gfx_initialized() ||
        !sokol_status->is_util_imgui_initialized() ||
        !sokol_status->is_sokol_initialized() || !sokol_status->is_spg_initialized()) {
      throw std::runtime_error("sokol is not fully initialized");
    }

    sokol_status->set_imgui_events_interception(true);
    scheduler->add_system(std::format("{}::RenderSokolBegin", module_name),
                          std::make_shared<systems::RenderSokolBegin>());
    scheduler->add_system(std::format("{}::RenderSokolDraw", module_name),
                          std::make_shared<systems::RenderSokolDraw>(map));
    scheduler->add_system(std::format("{}::RenderSokolEnd", module_name),
                          std::make_shared<systems::RenderSokolEnd>());
    scheduler->add_system(
      std::format("{}::SyncCameraWithMainWindow", module_name),
      std::make_shared<systems::SyncCameraWithMainWindow>());

    scheduler->add_system(std::format("{}::TextureLoader", module_name), std::make_shared<systems::TextureLoader>(locator.get_service<resource::Loader>()));
  }

} // namespace engine::graphics
