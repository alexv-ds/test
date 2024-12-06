#pragma once
#include <stdexcept>
#include <entt/entity/registry.hpp>

namespace engine
{
  class System
  {
  public:
    void init(std::shared_ptr<entt::registry> registry);
    virtual void start() {};
    virtual void stop() {};
    virtual void update() = 0;
    virtual ~System() = default;
  protected:
    [[nodiscard]] entt::registry &registry() const;

  private:
    std::shared_ptr<entt::registry> registry_;
  };

  // //////////////////////////// //
  // /////////// IMPL /////////// //
  // //////////////////////////// //
  inline entt::registry &System::registry() const
  {
    if (registry_ == nullptr) [[unlikely]]
    {
      throw std::runtime_error("registry is null");
    }
    return *registry_;
  }

}
