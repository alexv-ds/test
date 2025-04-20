#include "UpdateBBox.hpp"
#include <cmath>
#include "../components/world.hpp"
#include "../log.hpp"
#include "engine/components/graphics.hpp"

namespace engine::systems {
  using Rectangle = components::world::Rectangle;
  using BoundingBox = components::world::BoundingBox;
  using Scale = components::world::Scale;
  using Rotation = components::world::Rotation;

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
    reg.on_construct<Scale>().connect<&UpdateBBox::mark_for_update>(*this);
    reg.on_construct<Rotation>().connect<&UpdateBBox::mark_for_update>(*this);

    reg.on_update<Rectangle>().connect<&UpdateBBox::mark_for_update>(*this);
    reg.on_update<Scale>().connect<&UpdateBBox::mark_for_update>(*this);
    reg.on_update<Rotation>().connect<&UpdateBBox::mark_for_update>(*this);

    reg.on_destroy<Rectangle>().connect<&UpdateBBox::mark_for_update>(*this);
    reg.on_destroy<Scale>().connect<&UpdateBBox::mark_for_update>(*this);
    reg.on_destroy<Rotation>().connect<&UpdateBBox::mark_for_update>(*this);
  }

  void UpdateBBox::stop() {
    auto& reg = this->registry();

    reg.on_construct<Rectangle>().disconnect<&UpdateBBox::mark_for_update>(*this);
    reg.on_construct<Scale>().disconnect<&UpdateBBox::mark_for_update>(*this);
    reg.on_construct<Rotation>().disconnect<&UpdateBBox::mark_for_update>(*this);

    reg.on_update<Rectangle>().disconnect<&UpdateBBox::mark_for_update>(*this);
    reg.on_update<Scale>().disconnect<&UpdateBBox::mark_for_update>(*this);
    reg.on_update<Rotation>().disconnect<&UpdateBBox::mark_for_update>(*this);

    reg.on_destroy<Rectangle>().disconnect<&UpdateBBox::mark_for_update>(*this);
    reg.on_destroy<Scale>().disconnect<&UpdateBBox::mark_for_update>(*this);
    reg.on_destroy<Rotation>().disconnect<&UpdateBBox::mark_for_update>(*this);

    this->dirty_entities.clear();
    this->on_update_entities.clear();
  }

  void UpdateBBox::update() {
    auto& reg = this->registry();
    std::swap(this->dirty_entities, this->on_update_entities);
    for (const entt::entity e : this->on_update_entities) {
      update_bbox(reg, e);
    }
    on_update_entities.clear();
    std::swap(this->dirty_entities, this->on_update_entities);
    if (!on_update_entities.empty()) {
      LOG_WARN("During the BoundingBox update, {} entities were added to the update "
               "queue. They will not be updated.",
               on_update_entities.size());
      on_update_entities.clear();
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
    BoundingBox bbox = {
      .width = rect->width,
      .height = rect->height,
    };

    if (const auto* scale = reg.try_get<Scale>(e)) {
      bbox.width *= scale->x;
      bbox.height *= scale->y;
    }

    if (const auto* rotation = reg.try_get<Rotation>(e)) {
      const float width = bbox.width * std::abs(std::cos(rotation->theta)) +
        bbox.height * std::abs(std::sin(rotation->theta));
      const float height = bbox.width * std::abs(std::sin(rotation->theta)) +
        bbox.height * std::abs(std::cos(rotation->theta));
      bbox.width = width;
      bbox.height = height;
    }

    reg.emplace_or_replace<BoundingBox>(e, bbox);
  }

} // namespace engine::systems
