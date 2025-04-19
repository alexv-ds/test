#pragma once

namespace engine::components::graphics {

  struct Color {
    float r = 0;
    float g = 0;
    float b = 0;
  };

  struct Transparency {
    float a = 0;
  };

  struct Layer {
    float z = 0;
  };

  struct Invisible {};

  struct Camera {};

} // namespace engine::components::graphics
