#include <chrono>
#include <engine/Engine.hpp>
#include <engine/Map.hpp>
#include <engine/components/graphics.hpp>
#include <engine/components/other.hpp>
#include <engine/components/world.hpp>
#include <engine/hsv_rgb_conversion.hpp>
#include <engine/log.hpp>
#include <random>
#include <sokol_app.h>

#include "engine/Input.hpp"

void init(engine::ServiceRegistry& locator);

void engine_main(engine::ServiceRegistry& reg) {
  const std::shared_ptr lifecycle = reg.get_service<engine::EngineLifecycle>();
  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::init_post, init);
}

using namespace engine::components;

struct OrbitObject {
  float radius = 1;
  float angular_speed = 1;
  float angular_position = 0;
};

struct OrbitObjectGenerateOptions {
  world::Instance instance{};
  std::uniform_real_distribution<float> size;
  std::uniform_real_distribution<float> radius;
  std::uniform_real_distribution<float> angular_speed;
};

entt::entity create_object(entt::registry& reg,
                           OrbitObjectGenerateOptions& opts) {
  static std::default_random_engine random_engine{std::random_device{}()};
  static std::uniform_real_distribution<float> hue{0, 360};
  static std::uniform_real_distribution<float> angular_position{0, 2.0f * 3.1415926f};
  static std::uniform_real_distribution<float> layer{0, 1.0f};

  const entt::entity obj = reg.create();
  const auto size = opts.size(random_engine);
  const auto color = engine::hsv2rgb({
    .h = hue(random_engine),
    .s = 1.0f,
    .v = 1.0f
  });

  reg.emplace<world::Instance>(obj, opts.instance);
  reg.emplace<world::Rectangle>(obj, size, size);
  reg.emplace<world::Position>(obj);
  reg.emplace<world::Rotation>(obj);
  reg.emplace<graphics::Color>(obj, color.r, color.g, color.b);
  reg.emplace<OrbitObject>(obj, OrbitObject{
    .radius = opts.radius(random_engine),
    .angular_speed = opts.angular_speed(random_engine),
    .angular_position = angular_position(random_engine)
  });
  reg.emplace<graphics::Layer>(obj, layer(random_engine));
  reg.emplace<graphics::BlendMode>(obj, graphics::BlendMode::add);

  return obj;
}

void init(engine::ServiceRegistry& locator) {
  const std::shared_ptr scheduler = locator.get_service<engine::SystemScheduler>();
  const std::shared_ptr registry = locator.get_service<entt::registry>();
  const world::Instance instance{
    locator.get_service<engine::Map>()->create_instance("world").id};

  const auto camera = registry->create();
  registry->emplace<world::Instance>(camera, instance.id);
  registry->emplace<world::Rectangle>(camera, 4.0, 4.0);
  registry->emplace<world::Position>(camera);
  registry->emplace<graphics::Invisible>(camera);
  registry->emplace<other::InputWASDPositionController>(
    camera, other::InputWASDPositionController{.speed = 3, .shift_speed = 10});
  registry->emplace<graphics::Camera>(camera);
  registry->emplace<graphics::CameraLinkWithMainWindow>(
    camera, graphics::CameraLinkWithMainWindow{.preferred_height = 9});
  registry->emplace<world::Scale>(camera);


  scheduler->add_system("InputQEScaleController", [camera, &locator](entt::registry& reg) {
    if (!reg.valid(camera)) {
      return;
    }
    auto* p_scale = reg.try_get<world::Scale>(camera);
    if (!p_scale) {
      return;
    }

    auto input = locator.get_service<engine::Input>();
    constexpr float speed = 0.03;
    if (input->keyboard(engine::Input::KeyCode::q)) {
      p_scale->x += speed;
      p_scale->y += speed;
    }

    if (input->keyboard(engine::Input::KeyCode::e)) {
      p_scale->x -= speed;
      p_scale->y -= speed;
    }

    p_scale->x = std::max(p_scale->x, 0.0f);
    p_scale->y = std::max(p_scale->y, 0.0f);
    reg.patch<world::Scale>(camera);
  });

  scheduler->add_system("UpdatePosition", [](entt::registry& reg) {
    const auto view = reg.view<world::Position, OrbitObject>();
    const auto dt = static_cast<float>(sapp_frame_duration());

    for (const auto [e, pos, orbit] : view.each()) {
      orbit.angular_position +=
        dt * orbit.angular_speed / std::pow(std::max(orbit.radius, 0.0001f), 1.5f);
      pos.x = orbit.radius * std::cos(orbit.angular_position);
      pos.y = orbit.radius * std::sin(orbit.angular_position);

      reg.patch<world::Position>(e);
      reg.patch<OrbitObject>(e);
      if (auto* p_rotation = reg.try_get<world::Rotation>(e)) {
        p_rotation->theta = orbit.angular_position;
        reg.patch<world::Rotation>(e);
      }
    }
  });

  OrbitObjectGenerateOptions opts{
    .instance = instance,
    .size = std::uniform_real_distribution{0.1f, 0.3f},
    .radius = std::uniform_real_distribution{0.0001f,10.0f},
    .angular_speed = std::uniform_real_distribution{2.0f,2.6f}
  };
  for (int i = 0; i < 2300; ++i) {
    create_object(*registry, opts);
  }
}
