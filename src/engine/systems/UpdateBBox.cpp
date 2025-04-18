#include "../components/world.hpp"
#include "../log.hpp"
#include "UpdateBBox.hpp"

namespace engine::systems {
  using Rectangle = components::world::Rectangle;
  using BoundingBox = components::world::BoundingBox;

  // ////////////////////////////////// //
  // /////////// UpdateBBox /////////// //
  // ////////////////////////////////// //

  void UpdateBBox::start() {
    auto& reg = this->registry();
    // update already created entities
    for (const entt::entity entity : reg.view<Rectangle>()) {
      update_bbox(reg, entity);
    }

    reg.on_construct<Rectangle>().connect<&UpdateBBox::mark_for_update>(*this);
    reg.on_update<Rectangle>().connect<&UpdateBBox::mark_for_update>(*this);
    reg.on_destroy<Rectangle>().connect<&UpdateBBox::mark_for_update>(*this);
  }

  void UpdateBBox::stop() {
    auto& reg = this->registry();
    reg.on_construct<Rectangle>().disconnect<&UpdateBBox::mark_for_update>(*this);
    reg.on_update<Rectangle>().disconnect<&UpdateBBox::mark_for_update>(*this);
    reg.on_destroy<Rectangle>().disconnect<&UpdateBBox::mark_for_update>(*this);
  }

  void UpdateBBox::update() {
    auto& reg = this->registry();
    std::swap(this->dirty_entities, this->on_update_queue);
    for (const entt::entity e : this->on_update_queue) {
      update_bbox(reg, e);
    }
    on_update_queue.clear();
    std::swap(this->dirty_entities, this->on_update_queue);
    if (!on_update_queue.empty()) {
      LOG_WARN("During the BoundingBox update, {} entities were added to the update "
               "queue. They will not be updated.",
               on_update_queue.size());
      on_update_queue.clear();
    }
  }

  void UpdateBBox::mark_for_update(const entt::entity e) {
    this->dirty_entities.insert(e);
  }

  void UpdateBBox::update_bbox(entt::registry& reg, const entt::entity e) {
    if (!reg.valid(e)) {
      return;
    }
    const auto* rect = reg.try_get<Rectangle>(e);
    if (!rect) {
      return;
    }
    // TODO: добавить Rotation и Scale

    BoundingBox bbox = {
      .width = rect->width,
      .height = rect->height,
    };
    reg.emplace_or_replace<BoundingBox>(e, bbox);
  }

} // namespace engine::systems
