#pragma once
#include <functional>
#include <vector>
#include "ServiceRegistry.hpp"

namespace engine {

  enum class ModuleType { Standalone, /*Interface, Implementation*/ };

  struct ModuleDefine {
    ModuleType type = ModuleType::Standalone;
    std::string name{};
    std::string interface{};
    std::vector<std::string> require; // dependencies
    std::vector<std::string> optional; // optional dependencies
    std::function<void(ServiceRegistry&)> init = nullptr;
  };


} // namespace engine
