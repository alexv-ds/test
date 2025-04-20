#pragma once
#include <entt/entity/entity.hpp>
#include <memory>
#include <string_view>
#include <vector>
#include "components/world.hpp"

namespace engine {

  class Instance : std::enable_shared_from_this<Instance> {
  public:
    using Position = components::world::Position;
    using BoundingBox = components::world::BoundingBox;

    struct EntityData {
      Position position;
      BoundingBox bounding_box;
      entt::entity entity{};
    };

    const std::uint32_t id;

    explicit Instance(std::uint32_t id);
    virtual ~Instance() = default;
    virtual std::string_view name() const = 0;

    virtual void query(std::vector<EntityData>& out, const Position& pos,
                       const BoundingBox& bbox) const = 0;
    virtual void query(std::vector<entt::entity>& out, const Position& pos,
                       const BoundingBox& bbox) const = 0;
  };

  class Map {
  public:
    virtual ~Map() = default;
    virtual const Instance& create_instance(std::string_view name) = 0;
    virtual void delete_instance(std::uint32_t id) = 0;
    [[nodiscard]] virtual const Instance& get_instance(std::uint32_t id) const = 0;
    [[nodiscard]] virtual const Instance*
    try_get_instance(std::uint32_t id) const noexcept = 0;
  };

  // ////////////////////// //
  // //////// IMPL //////// //
  // ////////////////////// //

  inline Instance::Instance(const std::uint32_t id) : id(id) {}


} // namespace engine
