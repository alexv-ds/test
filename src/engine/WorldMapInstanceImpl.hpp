#pragma once
#include <cstdint>
#include <span>
#include <vector>
#include <entt/entity/entity.hpp>

namespace engine {

  class WorldMapInstanceImpl {
  public:

    // inline


  private:
    using MapCell = std::vector<entt::entity>;

    std::int32_t size_x_ = 0;
    std::int32_t size_y_ = 0;

    std::vector<entt::entity> entities_;

  };


} // namespace engine
