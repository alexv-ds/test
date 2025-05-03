#pragma once
#include <memory>
#include "../Map.hpp"
#include "../System.hpp"

namespace engine::systems {
  class RenderSokolDraw final : public System {
  public:
    explicit RenderSokolDraw(std::shared_ptr<Map> map) : map(std::move(map)) {}
    void update() override;

  private:
    void render() const;

    std::shared_ptr<Map> map;

    // struct TextureDeleter =

    // using rexture_ptr = std::shared_ptr<

  };
} // namespace engine::systems
