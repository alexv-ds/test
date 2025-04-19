#pragma once
#include <entt/entity/entity.hpp>
#include <string>
#include <vector>
#include "Map.hpp"
#include "noncopyable.hpp"
// #include <loose_quadtree/loose_quadtree.hpp>
#include "components/world.hpp"

namespace engine {

  class InstanceImpl;

  struct TreeObject {
    entt::entity entity= entt::null;
    components::world::BoundingBox bbox{};
    components::world::Position position{};
    components::world::Instance instance{};
  };


  class MapImpl final : public Map, public noncopyable {
  public:
    MapImpl();
    ~MapImpl() override;
    const Instance& create_instance(std::string_view name) override;
    void delete_instance(std::uint32_t id) override;
    [[nodiscard]] const Instance& get_instance(std::uint32_t id) const override;
    [[nodiscard]] const Instance*
    try_get_instance(std::uint32_t id) const noexcept override;

    void update_tree_object(TreeObject* obj);
    void remove_tree_object(TreeObject* obj);

  private:
    [[nodiscard]] InstanceImpl* try_get_impl_instance(std::uint32_t id) const noexcept;

  private:
    std::vector<std::shared_ptr<InstanceImpl>> instances;
    std::vector<entt::entity> entities_for_delete;
  };

} // namespace engine
