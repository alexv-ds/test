#include "InputQEScaleController.hpp"
#include "../components/other.hpp"
#include "../components/world.hpp"
#include <cmath>
#include <sokol_app.h>

namespace engine::systems {
  using Scale = components::world::Scale;
  using Controller = components::other::InputQEScaleController;
  using KeyCode = Input::KeyCode;

  void InputQEScaleController(entt::registry& reg, std::shared_ptr<Input> input) {
    float direction = 0;
    bool shift =
      input->keyboard(KeyCode::left_shift) || input->keyboard(KeyCode::right_shift);
    if (input->keyboard(KeyCode::q)) {
      direction = 1;
    }
    if (input->keyboard(KeyCode::e)) {
      direction = -1;
    }
    if (direction == 0) {
      return;
    }

    direction *= static_cast<float>(sapp_frame_duration());
    const auto view = reg.view<Scale, const Controller>();
    for (const auto [e, scale, controller] : view.each()) {
      scale.x += (shift ? controller.shift_speed : controller.speed) * direction;
      scale.y += (shift ? controller.shift_speed : controller.speed) * direction;
      scale.x = std::max(scale.x, 0.01f);
      scale.y = std::max(scale.y, 0.01f);
      reg.patch<Scale>(e);
    }
  }

} // namespace engine::systems
