#pragma once
#include <set>
#include "../System.hpp"

namespace engine::systems {

  class UpdateBBox final : public System {
  public:
    void start() override;
    void stop() override;
    void update() override;

  private:
    std::set<entt::entity> dirty_entities;
    std::set<entt::entity> on_update_entities;

    void mark_for_update(entt::entity e);
    static void update_bbox(entt::registry& reg, entt::entity e);
  };

} // namespace engine::systems
