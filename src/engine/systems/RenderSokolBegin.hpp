#pragma once
#include <memory>
#include "../System.hpp"

namespace engine::systems {
  class RenderSokolBegin final : public System {
  public:
    void update() override;
  };
} // namespace engine::systems
