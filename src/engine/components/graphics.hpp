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

  struct CameraLinkWithMainWindow {
    float preferred_height = 15;
  };

  struct CameraCopyPositionFrom {
    entt::entity target = entt::null;
  };

  struct BlendMode {
    enum Mode {
      alpha,
      add,
      modulate,
      multiply
    };
    Mode mode = alpha;
  };

} // namespace engine::components::graphics
