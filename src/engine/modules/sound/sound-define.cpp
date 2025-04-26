#include <engine/modules/resource.hpp>
#include <engine/modules/sound.hpp>

namespace engine::sound {

  const ModuleDefine module_define = ModuleDefineStandalone{
    .name = module_name,
    .require = {resource::module_name},
    .optional = {}
  };

}