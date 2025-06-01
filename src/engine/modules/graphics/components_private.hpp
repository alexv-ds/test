#pragma once
#include <memory>
#include <sokol_gfx.h>
#include <sokol_gp.h>
#include <engine/modules/resource/services/Loader.hpp>

namespace engine::graphics::detail::components {
  struct Texture {
    std::shared_ptr<sg_image> texture;
    sgp_rect uv;
  };

  struct TextureLoader {
    std::shared_ptr<resource::Resource> resource;
  };

}
