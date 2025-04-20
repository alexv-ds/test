#include "UpdateMap.hpp"
#include <format>
#include "../components/world.hpp"
#include "../log.hpp"
#include "../noncopyable.hpp"


namespace engine::systems {
  using Position = components::world::Position;
  using BoundingBox = components::world::BoundingBox;
  using Instance = components::world::Instance;

  namespace {

    /// @warning when updating this component, we do not use patch<TreeObjectComponent>()
    class TreeObjectComponent final : public noncopyable {
    public:
      explicit TreeObjectComponent(std::shared_ptr<MapImpl> map_) :
          map(std::move(map_)) {};

      TreeObjectComponent(TreeObjectComponent&& other) noexcept :
          obj(std::move(other.obj)),
          map(other.map) // NOLINT(*-move-constructor-init)
                         // Map needs to be copied. The
                         // object must remain valid when calling update()
      {
        other.obj = nullptr;
      }

      ~TreeObjectComponent() {
        if (obj) {
          map->remove_tree_object(this->obj.get());
        }
      }

      void update(const entt::entity entity, const BoundingBox& bbox,
                  const Position& position, const Instance& instance) {
        if (!this->obj) {
          this->obj = std::make_unique<TreeObject>(entity, bbox, position, instance);
        }
        if (this->obj->instance.id != instance.id) {
          map->remove_tree_object(this->obj.get());
        }
        this->obj->entity = entity;
        this->obj->bbox = bbox;
        this->obj->position = position;
        this->obj->instance = instance;
        map->update_tree_object(this->obj.get());
      }

    private:
      std::unique_ptr<TreeObject> obj = nullptr;
      std::shared_ptr<MapImpl> map;
    };

  } // namespace

  void UpdateMap::start() {
    this->map->clean_tree_objects();
    this->dirty_entities.clear();

    const auto view =
      registry().view<const Position, const BoundingBox, const Instance>();
    for (const auto e : view) {
      this->insert_into_dirty(e);
    }

    registry().on_construct<Position>().connect<&UpdateMap::insert_into_dirty>(this);
    registry().on_construct<BoundingBox>().connect<&UpdateMap::insert_into_dirty>(this);
    registry().on_construct<Instance>().connect<&UpdateMap::insert_into_dirty>(this);

    registry().on_update<Position>().connect<&UpdateMap::insert_into_dirty>(this);
    registry().on_update<BoundingBox>().connect<&UpdateMap::insert_into_dirty>(this);
    registry().on_update<Instance>().connect<&UpdateMap::insert_into_dirty>(this);

    registry().on_destroy<Position>().connect<&UpdateMap::insert_into_dirty>(this);
    registry().on_destroy<BoundingBox>().connect<&UpdateMap::insert_into_dirty>(this);
    registry().on_destroy<Instance>().connect<&UpdateMap::insert_into_dirty>(this);
  }

  void UpdateMap::stop() {
    registry().on_construct<Position>().disconnect<&UpdateMap::insert_into_dirty>(this);
    registry().on_construct<BoundingBox>().disconnect<&UpdateMap::insert_into_dirty>(
      this);
    registry().on_construct<Instance>().disconnect<&UpdateMap::insert_into_dirty>(this);

    registry().on_update<Position>().disconnect<&UpdateMap::insert_into_dirty>(this);
    registry().on_update<BoundingBox>().disconnect<&UpdateMap::insert_into_dirty>(this);
    registry().on_update<Instance>().disconnect<&UpdateMap::insert_into_dirty>(this);

    registry().on_destroy<Position>().disconnect<&UpdateMap::insert_into_dirty>(this);
    registry().on_destroy<BoundingBox>().disconnect<&UpdateMap::insert_into_dirty>(this);
    registry().on_destroy<Instance>().disconnect<&UpdateMap::insert_into_dirty>(this);

    this->dirty_entities.clear();
  }

  void UpdateMap::insert_into_dirty(entt::entity e) {
    if (this->on_update) {
      throw std::logic_error(
        std::format("cannot add entity-{} to update set: update in progress",
                    static_cast<std::underlying_type_t<entt::entity>>(e)));
    }
    this->dirty_entities.insert(e);
  }

  void UpdateMap::update() {
    this->on_update = true;
    for (const auto& e : this->dirty_entities) {
      try {
        entt::registry& reg = registry();
        if (!reg.valid(e)) {
          continue;
        }
        const Position* pos = reg.try_get<Position>(e);
        const BoundingBox* bbox = reg.try_get<BoundingBox>(e);
        const Instance* instance = reg.try_get<Instance>(e);
        TreeObjectComponent* tree_object_component =
          reg.try_get<TreeObjectComponent>(e);

        if (tree_object_component) {
          if (pos && bbox && instance) {
            tree_object_component->update(e, *bbox, *pos, *instance);
          } else {
            reg.remove<TreeObjectComponent>(e);
          }
        } else if (pos && bbox && instance) {
          TreeObjectComponent new_tree_object_component{this->map};
          new_tree_object_component.update(e, *bbox, *pos, *instance);
          reg.emplace<TreeObjectComponent>(e, std::move(new_tree_object_component));
        }
      }
      catch (const std::exception& ex) {
        LOG_ERROR("cannot update entity-{} on map: {}",
                  static_cast<std::underlying_type_t<entt::entity>>(e), ex.what());
      }
    }
    this->on_update = false;
    this->dirty_entities.clear();
  }

} // namespace engine::systems
