#pragma once

namespace engine::components::other {
  struct InputWASDPositionController {
    float speed = 1; // per second
    float shift_speed = 3; // per second
  };

  struct InputQEScaleController {
    float speed = 1; // per second
    float shift_speed = 3; // per second
  };
}