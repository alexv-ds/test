#include "RenderSokolDraw.hpp"
#include <optional>
#include <sokol_gfx.h>
#include <sokol_gp.h>
#include "../components/graphics.hpp"
#include "../components/world.hpp"
#include "../log.hpp"

namespace engine::systems {

  using namespace components;

  struct DrawObject {
    world::Rectangle rectangle{};
    world::Position position{};
    world::Rotation rotation{
      .theta = 0,
    };
    world::Scale scale{.x = 1.0f, .y = 1.0f};
    graphics::Color color{.r = 1.0f, .g = 1.0f, .b = 1.0f};
    graphics::Transparency transparency{.a = 1.0f};
    graphics::Layer layer{.z = 0};
    std::optional<graphics::BlendMode> blend;
  };


  void RenderSokolDraw::update() {
    this->render();
  }

  // TODO: VERY VERY VERY SIMPLE AND STUPID - REWRITE ME
  void RenderSokolDraw::render() const {
    constexpr int texture_channel = 0;
    sgp_reset_image(texture_channel);

    entt::registry& reg = registry();
    float camera_width{};
    float camera_height{};
    world::Position camera_position;
    world::Instance camera_instance;
    world::BoundingBox camera_bbox;
    // Get camera
    {
      const auto view =
        reg.view<const world::Rectangle, const world::Position, const world::Instance,
                 const world::BoundingBox, const graphics::CameraLinkWithMainWindow,
                 const graphics::Camera>();

      bool has_camera = false;
      for (auto [e, rect, pos, instance, bbox, _] : view.each()) {
        if (has_camera) {
          LOG_ERROR("not unique entity-{} with CameraLinkWithMainWindow component has "
                    "been deleted",
                    static_cast<std::underlying_type_t<entt::entity>>(e));
          reg.destroy(e);
        }
        has_camera = true;
        world::Scale scale{1, 1};
        if (const auto* p_scale = reg.try_get<world::Scale>(e)) {
          scale = *p_scale;
        }

        camera_width = rect.width * scale.x;
        camera_height = rect.height * scale.y;
        camera_position = pos;
        camera_instance = instance;
        camera_bbox = bbox;
      }
      if (!has_camera) {
        return;
      }
    }

    if (camera_width <= 0.01f || camera_height <= 0.01f) {
      return;
    }


    thread_local std::vector<DrawObject> draw_objects;
    thread_local std::vector<Instance::EntityData> map_query_buffer;
    draw_objects.clear();
    map_query_buffer.clear();
    // fill vector
    {
      auto instance = this->map->try_get_instance(camera_instance.id);
      if (!instance) {
        return;
      }
      instance->query(map_query_buffer, camera_position, camera_bbox);

      for (const Instance::EntityData& data : map_query_buffer) {
        if (!reg.valid(data.entity)) {
          continue;
        }

        if (reg.all_of<graphics::Invisible>(data.entity)) {
          continue;
        }

        DrawObject draw_object{};
        draw_object.position = data.position;
        if (const auto* p_rect = reg.try_get<world::Rectangle>(data.entity)) {
          draw_object.rectangle = *p_rect;
        }
        else {
          continue;
        }
        if (const auto* p_rotation = reg.try_get<world::Rotation>(data.entity)) {
          draw_object.rotation = *p_rotation;
        }
        if (const auto* p_scale = reg.try_get<world::Scale>(data.entity)) {
          draw_object.scale = *p_scale;
        }
        if (const auto* p_color = reg.try_get<graphics::Color>(data.entity)) {
          draw_object.color = *p_color;
          draw_object.color.r = std::clamp(draw_object.color.r, 0.0f, 1.0f);
          draw_object.color.g = std::clamp(draw_object.color.g, 0.0f, 1.0f);
          draw_object.color.b = std::clamp(draw_object.color.b, 0.0f, 1.0f);
        }
        if (const auto* p_layer = reg.try_get<graphics::Layer>(data.entity)) {
          draw_object.layer = *p_layer;
        }
        if (const auto p_transparency =
              reg.try_get<graphics::Transparency>(data.entity)) {
          draw_object.transparency = *p_transparency;
        }
        if (const auto p_blend_mode = reg.try_get<graphics::BlendMode>(data.entity)) {
          draw_object.blend = *p_blend_mode;
        }
        draw_objects.push_back(draw_object);
      }

      std::sort(draw_objects.begin(), draw_objects.end(),
                [](const auto& a, const auto& b) { return a.layer.z < b.layer.z; });

      sgp_project(-camera_width * 0.5f + camera_position.x,
                  camera_width * 0.5f + camera_position.x,
                  camera_height * 0.5f + camera_position.y,
                  -camera_height * 0.5f + camera_position.y);


      constexpr auto engine_to_sokol_blend = [](const graphics::BlendMode::Mode mode) {
        switch (mode) {
        case graphics::BlendMode::add:
          return SGP_BLENDMODE_ADD;
        case graphics::BlendMode::modulate:
          return SGP_BLENDMODE_MOD;
        case graphics::BlendMode::multiply:
          return SGP_BLENDMODE_MUL;
        default:
          return SGP_BLENDMODE_BLEND;
        }
      };

      for (const auto& obj : draw_objects) {
        if (obj.blend) {
          sgp_set_blend_mode(engine_to_sokol_blend(obj.blend->mode));
        }
        else {
          sgp_set_blend_mode(SGP_BLENDMODE_NONE);
        }
        sgp_set_color(obj.color.r, obj.color.g, obj.color.b, obj.transparency.a);
        sgp_push_transform();
        sgp_translate(obj.position.x - obj.rectangle.width * obj.scale.x * 0.5f,
                      obj.position.y - obj.rectangle.height * obj.scale.x * 0.5f);
        sgp_scale(obj.scale.x, obj.scale.y);
        sgp_rotate_at(obj.rotation.theta, obj.rectangle.width * 0.5f,
                      obj.rectangle.height * 0.5f);

        // sgp_set_image()
        {
          const sgp_rect dest{
            .x = 0.f,
            .y = 0.f,
            .w = obj.rectangle.width,
            .h = obj.rectangle.height,
          };
          const sgp_rect src{
            .x = 0.f,
            .y = 0.f,
            .w = 1.f,
            .h = 1.f,
          };
          sgp_draw_textured_rect(texture_channel, dest, src);
          sgp_reset_image(texture_channel);
        }
        // sgp_draw_filled_rect(0.f, 0.f, obj.rectangle.width, obj.rectangle.height);
        sgp_pop_transform();
        sgp_reset_color();
        sgp_reset_blend_mode();
      }
      sgp_reset_project();
    }
  }

} // namespace engine::systems
