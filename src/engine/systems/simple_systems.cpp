#include <entt/entity/registry.hpp>
#include "../components/world.hpp"
#include "simple_systems.hpp"

using namespace engine::components;

using registry = entt::registry;

static void CopyPositionFrom(registry& reg) {
  for (auto [e, copy] : reg.view<const world::CopyPositionFrom>().each()) {
    if (!reg.valid(copy.target)) {
      continue;
    }

    const auto* p_target_pos = reg.try_get<world::Position>(copy.target);
    const auto* p_self_pos = reg.try_get<world::Position>(e);

    if (!p_target_pos && !p_self_pos) {
      return;
    }

    if (!p_target_pos) {
      reg.remove<world::Position>(e);
      return;
    }

    if (!p_self_pos) {
      reg.emplace<world::Position>(e, *p_target_pos);
    }

    if (p_self_pos) {
      if (p_target_pos->x != p_self_pos->x || p_target_pos->y != p_self_pos->y) {
        reg.replace<world::Position>(e, *p_target_pos);
      }
    }
  }
}

void engine::systems::add_simple_systems(SystemScheduler& scheduler) {
  scheduler.add_system("CopyPositionFrom", CopyPositionFrom);
}
