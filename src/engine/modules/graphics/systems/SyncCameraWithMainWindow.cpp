#include "SyncCameraWithMainWindow.hpp"
#include <engine/components/world.hpp>
#include <engine/modules/graphics/components.hpp>
#include <sokol_app.h>


namespace engine::graphics::systems {

  using namespace engine::components;

  void SyncCameraWithMainWindow::update() {
    auto& reg = registry();
    const float ratio = sapp_widthf() / sapp_heightf();

    for (const auto [e, rect, sync] :
         reg.view<const world::Rectangle, const components::CameraLinkWithMainWindow>()
           .each()) {
      const world::Rectangle new_rect{
        .width = sync.preferred_height * ratio,
        .height = sync.preferred_height,
      };
      if (new_rect.height != rect.height || new_rect.width != rect.width) {
        reg.replace<world::Rectangle>(e, new_rect);
      }
    }
  }
} // namespace engine::graphics::systems
