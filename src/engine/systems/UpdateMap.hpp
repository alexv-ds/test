#pragma once
#include <unordered_set>
#include "../MapImpl.hpp"
#include "../System.hpp"

namespace engine::systems {

  class UpdateMap final : public System {
  public:
    explicit UpdateMap(std::shared_ptr<MapImpl> map) : map(std::move(map)) {}
    void start() override;
    void update() override;
    void stop() override;

  private:
    std::shared_ptr<MapImpl> map;

    bool on_update = false;
    std::unordered_set<entt::entity> dirty_entities;

    void insert_into_dirty(entt::entity e);

  };

} // namespace engine::systems
