#pragma once
#include <engine/System.hpp>

namespace engine::graphics::systems {

  class SyncCameraWithMainWindow final : public System {
  public:
    void update() override;
  };

} // namespace engine::graphics::systems
