#pragma once
#include <map>
#include <set>
#include "ModuleLoader.hpp"
#include "ModuleRegistry.hpp"
#include "noncopyable.hpp"
#include <expected>

namespace engine {

  class ModuleRegistryImpl final : public ModuleRegistry,
                                   public ModuleLoader,
                                   public noncopyable {
  public:
    explicit ModuleRegistryImpl(std::shared_ptr<ServiceRegistry> locator) :
        locator(std::move(locator)) {}
    void define(const ModuleDefine& define) override;
    void load(const std::string& name) override;

  private:

    std::map<std::string, ModuleDefine> defines;
    std::set<std::string> loaded_modules;
    bool load_in_progress = false;
    std::shared_ptr<ServiceRegistry> locator;

    bool recursive_load(const std::string& name, int depth = 0);
  };

} // namespace engine
