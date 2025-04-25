#pragma once
#include <string>

namespace engine {

  class ModuleLoader {
  public:
    virtual ~ModuleLoader() = default;
    virtual void load(const std::string& name) = 0;
  };

} // namespace engine
