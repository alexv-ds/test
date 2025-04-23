#pragma once
#include <cstdint>

namespace engine::components::world {

  struct Instance {
    std::uint32_t id = 0;
  };

  struct Position {
    float x = 0;
    float y = 0;
  };

  struct Scale {
    float x = 1;
    float y = 1;
  };

  struct Rotation {
    float theta = 0; // rad
  };

  struct Rectangle {
    float width = 1;
    float height = 1;
  };

  struct BoundingBox {
    float width = 0;
    float height = 0;
  };

  struct CopyPositionFrom {
    entt::entity target = entt::null;
  };

} // namespace engine::components::world
