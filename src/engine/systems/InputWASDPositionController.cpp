#include "InputWASDPositionController.hpp"
#include <sokol_app.h>
#include "../components/other.hpp"
#include "../components/world.hpp"

// #include

namespace engine::systems {
  using KeyCode = Input::KeyCode;
  using Position = components::world::Position;
  using CompWASD = components::other::InputWASDPositionController;

  void InputWASDPositionController::update() {
    float dx = 0;
    float dy = 0;
    bool shift =
      input->keyboard(KeyCode::left_shift) || input->keyboard(KeyCode::right_shift);

    if (input->keyboard(KeyCode::w)) {
      dy += 1.0f;
    }
    if (input->keyboard(KeyCode::s)) {
      dy -= 1.0f;
    }
    if (input->keyboard(KeyCode::a)) {
      dx -= 1.0f;
    }
    if (input->keyboard(KeyCode::d)) {
      dx += 1.0f;
    }

    if (dx == 0 && dy == 0) {
      return;
    }

    const auto frame_duration = static_cast<float>(sapp_frame_duration());
    for (const auto [e, pos, opts] : registry().view<Position, const CompWASD>().each()) {
      const float factor = (shift ? opts.shift_speed : opts.speed) * frame_duration;
      pos.x += dx * factor;
      pos.y += dy * factor;
    }
  }
} // namespace engine::systems
