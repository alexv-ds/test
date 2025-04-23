#pragma once
#include <entt/entity/registry.hpp>
#include "../Input.hpp"

namespace engine::systems {

  void InputQEScaleController(entt::registry& reg, std::shared_ptr<Input> input);

}