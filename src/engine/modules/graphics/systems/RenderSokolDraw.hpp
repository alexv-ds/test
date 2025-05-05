#pragma once
#include <memory>
#include <engine/Map.hpp>
#include <engine/System.hpp>

namespace engine::graphics::systems {
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
