#pragma once
#include "ModuleDefine.hpp"

namespace engine {

  class ModuleRegistry {
  public:
    virtual ~ModuleRegistry() = default;
    virtual void define(const ModuleDefine& define) = 0;
  };

} // namespace engine
