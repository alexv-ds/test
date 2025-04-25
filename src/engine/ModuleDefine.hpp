#pragma once
#include <functional>
#include <vector>
#include "ServiceRegistry.hpp"

namespace engine {

  enum class ModuleType { Standalone, /*Interface, Implementation*/ };

  struct ModuleDefineStandalone {
    std::string name{};
    std::vector<std::string> require;
    std::vector<std::string> optional;
    std::function<void(ServiceRegistry&)> init = nullptr;
  };

  struct ModuleDefine {
    ModuleType type = ModuleType::Standalone;
    std::string name{};
    std::string interface{};
    std::vector<std::string> require; // dependencies
    std::vector<std::string> optional; // optional dependencies
    std::function<void(ServiceRegistry&)> init = nullptr;

    ModuleDefine() = default;
    // ReSharper disable once CppNonExplicitConvertingConstructor
    ModuleDefine(ModuleDefineStandalone define) { // NOLINT(*-explicit-constructor)
      this->type = ModuleType::Standalone;
      this->name = std::move(define.name);
      this->require = std::move(define.require);
      this->optional = std::move(define.optional);
      this->init = std::move(define.init);
    }
  };


} // namespace engine
