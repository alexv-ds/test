#pragma once

namespace engine::components::other {
  struct InputWASDPositionController {
    float speed = 1; // per second
    float shift_speed = 0.1f; // per second
  };
}