#pragma once
#include "../System.hpp"

namespace engine::systems {
  class RenderSokolEnd final : public System {
  public:
    void update() override;
  };
} // namespace engine::systems
