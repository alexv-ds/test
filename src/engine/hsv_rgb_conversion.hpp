#pragma once

namespace engine {

  struct Rgb {
    float r = 0; // [0,1]
    float g = 0; // [0,1]
    float b = 0; // [0,1]
  };

  struct Hsv {
    float h = 0; // [0,360]
    float s = 0; // [0,1]
    float v = 0; // [0,1]
  };

  Rgb hsv2rgb(Hsv hsv) noexcept;
  Hsv rgb2hsv(Rgb rgb) noexcept;
}
