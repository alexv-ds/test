#include <engine/modules/graphics.hpp>
#include <engine/modules/resource.hpp>

namespace engine::graphics {

  const ModuleDefine module_define = ModuleDefineStandalone{
    .name = module_name,
    .require = {resource::module_name},
    .optional = {},
    .init = nullptr,
  };


}
