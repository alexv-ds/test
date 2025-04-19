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
    float x = 0;
    float y = 0;
  };

  struct Rotation {
    float theta = 0; // rad
  };

  struct Rectangle {
    float width = 0;
    float height = 0;
  };

  struct BoundingBox {
    float width = 0;
    float height = 0;
  };

} // namespace engine::components::world
