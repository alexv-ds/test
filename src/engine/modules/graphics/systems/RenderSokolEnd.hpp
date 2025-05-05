#pragma once
#include <engine/System.hpp>

namespace engine::graphics::systems {
  class RenderSokolEnd final : public System {
  public:
    void update() override;
  };
} // namespace engine::graphics::systems
