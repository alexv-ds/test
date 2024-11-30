#pragma once
#include <entt/entity/registry.hpp>

namespace engine
{
  class Engine
  {
  public:
    Engine(const Engine &) = delete;
    Engine &operator=(const Engine &) = delete;
  private:
    std::shared_ptr<entt::registry> registry;
  };
}
