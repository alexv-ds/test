#include <chrono>
#include <engine/Engine.hpp>
#include <engine/Map.hpp>
#include <engine/components/graphics.hpp>
#include <engine/components/other.hpp>
#include <engine/components/world.hpp>
#include <engine/hsv_rgb_conversion.hpp>
#include <engine/log.hpp>
#include <imgui.h>
#include <random>
#include <sokol_app.h>
// #include <soloud.h>
// #include <soloud_wav.h>
#include "engine/Input.hpp"


void init(engine::ServiceRegistry& locator);

void engine_main(engine::ServiceRegistry& reg) {
  const std::shared_ptr lifecycle = reg.get_service<engine::EngineLifecycle>();
  lifecycle->add_callback_static(engine::EngineLifecycle::Stage::init_post, init);
}

using namespace engine::components;

void ImguiTest(entt::registry&);

void init(engine::ServiceRegistry& locator) {
  const std::shared_ptr scheduler = locator.get_service<engine::SystemScheduler>();
  const std::shared_ptr registry = locator.get_service<entt::registry>();
  const world::Instance instance{
    locator.get_service<engine::Map>()->create_instance("world").id};


  const auto i_am = registry->create();
  registry->emplace<world::Instance>(i_am, instance.id);
  registry->emplace<world::Rectangle>(i_am, 1, 1);
  registry->emplace<world::Position>(i_am);
  registry->emplace<graphics::Color>(i_am, 1, 0, 0);
  registry->emplace<other::InputWASDPositionController>(
    i_am, other::InputWASDPositionController{.speed = 3, .shift_speed = 10});
  registry->emplace<graphics::Layer>(i_am, 10);

  const auto camera = registry->create();
  registry->emplace<world::Instance>(camera, instance.id);
  registry->emplace<world::Rectangle>(camera);
  registry->emplace<world::Position>(camera);
  registry->emplace<graphics::Invisible>(camera);
  registry->emplace<other::InputQEScaleController>(camera);
  registry->emplace<graphics::Camera>(camera);
  registry->emplace<graphics::CameraLinkWithMainWindow>(
    camera, graphics::CameraLinkWithMainWindow{.preferred_height = 14});
  registry->emplace<world::Scale>(camera);
  registry->emplace<world::CopyPositionFrom>(camera, i_am);


  for (int x = -20; x <= 20; x += 2) {
    for (int y = -20; y <= 20; y += 2) {
      const auto e = registry->create();
      registry->emplace<world::Instance>(e, instance.id);
      registry->emplace<world::Rectangle>(e, 1, 1);
      registry->emplace<world::Position>(e, x, y);
    }
  }

  // scheduler->add_system("ImguiTest", ImguiTest);
}

/*
void ImguiTest(entt::registry&) {
  static auto soloud = [] {
    // No leak, we place it into unique_ptr
    // ReSharper disable once CppDFAMemoryLeak
    auto* engine = new SoLoud::Soloud();
    if (const auto err = engine->init(); err) {
      LOG_CRITICAL("Cannot init soloud: {}", engine->getErrorString(err));
      std::exit(EXIT_FAILURE);
    }
    std::unique_ptr<SoLoud::Soloud, void (*)(SoLoud::Soloud*)> unique_engine{
      engine, [](SoLoud::Soloud* raw_engine) {
        raw_engine->deinit();
        delete raw_engine;
      }};
    return unique_engine;
  }();

  ImGui::Begin("Hello world");
  ImGui::Text("Hello, world %d", 123);
  if (ImGui::Button("BOOOP")) {
    auto* sample = new(SoLoud::Wav);
    if (const auto err = sample->load("resources/sad-violin.mp3"); err) {
      LOG_ERROR("cannot load mp3: {}", soloud->getErrorString(err));
      std::exit(EXIT_FAILURE);
    }
    auto handle = soloud->play(*sample);
  }
  // ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
  // ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

  ImGui::End();
}
*/
