#include <engine/log.hpp>
#include "../resource.hpp"

namespace engine::resource {

  static void init(ServiceRegistry&);

  const ModuleDefine module_define = ModuleDefineStandalone{
    .name = module_name,
    .require = {},
    .optional = {},
    .init = init,
  };

  static void init(ServiceRegistry&) { LOG_INFO("HELLO FROM MODULE"); }


} // namespace engine::resource
