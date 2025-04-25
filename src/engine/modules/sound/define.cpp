#include "define.hpp"

namespace engine::sound {

  const std::string module_name = "engine::sound";

  const ModuleDefine module_define = ModuleDefineStandalone{
    .name = module_name,
    .require = {},
    .optional = {}
  };


}