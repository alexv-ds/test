#include "SyncCameraWithMainWindow.hpp"
#include "../components/graphics.hpp"
#include "../components/world.hpp"
#include <sokol_app.h>


namespace engine::systems {

  using namespace components;

  void SyncCameraWithMainWindow::update() {
    auto& reg = registry();
    const float ratio = sapp_widthf() / sapp_heightf();

    for (const auto [e, rect, sync] :
         reg.view<const world::Rectangle, const graphics::CameraLinkWithMainWindow>().each()) {
      const world::Rectangle new_rect {
        .width = sync.preferred_height * ratio,
        .height = sync.preferred_height,
      };
      if (new_rect.height != rect.height || new_rect.width != rect.width) {
        reg.replace<world::Rectangle>(e, new_rect);
      }
    }
  }
} // namespace engine::systems
