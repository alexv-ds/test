#include <engine/SokolStatus.hpp>
#include <engine/SystemScheduler.hpp>
#include <engine/log.hpp>
#include "../resource.hpp"
#include "services/Loader.hpp"

namespace engine::resource {

  static void init(ServiceRegistry&);

  const ModuleDefine module_define = ModuleDefineStandalone{
    .name = module_name,
    .require = {},
    .optional = {},
    .init = init,
  };

  static void init(ServiceRegistry& locator) {
    if (!locator.get_service<SokolStatus>()->is_sokol_initialized()) {
      throw std::runtime_error("sokol is not initialized");
    }

    const auto scheduler = locator.get_service<SystemScheduler>();

    // scheduler->add_system(std::format("{}::ResourceLoader", module_name),
    //                       std::make_shared<systems::ResourceLoader>());


    locator.add_service(std::make_shared<Loader>(locator.get_service<EngineLifecycle>()));
  }


} // namespace engine::resource
