#include "ModuleRegistryImpl.hpp"
#include <format>
#include "log.hpp"

namespace engine {

  void ModuleRegistryImpl::load(const std::string& name) {
    bool loaded;
    this->load_in_progress = true;
    try {
      loaded = this->recursive_load(name);
      this->load_in_progress = false;
    }
    catch (...) {
      this->load_in_progress = false;
      std::rethrow_exception(std::current_exception());
    }

    if (!loaded) {
      throw std::runtime_error(std::format("failed to load module '{}'", name));
    }
  }

  void ModuleRegistryImpl::define(const ModuleDefine& define) {
    if (this->load_in_progress) {
      throw std::logic_error(std::format(
        "cannot define a module '{}' while instantiating another module", define.name));
    }
    if (this->defines.contains(define.name)) {
      throw std::logic_error(std::format("module '{}' already defined", define.name));
    }
    this->defines.insert({define.name, define});
  }

  bool
  ModuleRegistryImpl::recursive_load(const std::string& name, // NOLINT(*-no-recursion)
                                     const int depth) {
    if (constexpr int max_depth = 5; depth > max_depth) {
      throw std::logic_error(std::format("max depth {} reached", max_depth));
    }

    LOG_TRACE("loading module '{}'", name);

    const auto define = this->defines.find(name);
    if (define == this->defines.end()) {
      return false;
    }
    for (const auto& opt : define->second.optional) {
      recursive_load(opt, depth + 1);
    }
    for (const auto& req : define->second.require) {
      if (!recursive_load(req, depth + 1)) {
        return false;
      }
    }
    if (define->second.init) {
      define->second.init(*this->locator);
    }
    this->loaded_modules.insert(define->second.name);
    LOG_INFO("module loaded - '{}'", name);
    return true;
  }

} // namespace engine
