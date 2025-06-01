#include "TextureLoader.hpp"

#include "engine/modules/graphics/components.hpp"


namespace engine::graphics::systems {
  using Sprite = components::Sprite;

  void TextureLoader::start() {
    auto& reg = registry();
    reg.on_construct<Sprite>().connect<&TextureLoader::add_to_dirty>(this);
    reg.on_update<Sprite>().connect<&TextureLoader::add_to_dirty>(this);
    reg.on_destroy<Sprite>().connect<&TextureLoader::add_to_dirty>(this);
  }

  void TextureLoader::stop() {
    auto& reg = registry();
    reg.on_construct<Sprite>().disconnect<&TextureLoader::add_to_dirty>(this);
    reg.on_update<Sprite>().disconnect<&TextureLoader::add_to_dirty>(this);
    reg.on_destroy<Sprite>().disconnect<&TextureLoader::add_to_dirty>(this);
  }

  void TextureLoader::update() {

  }

  void TextureLoader::add_to_dirty(const entt::entity e) {
    this->dirty_entities.insert(e);
    LOG_INFO("HI DUDDLES");
  }

  std::shared_ptr<sg_image> TextureLoader::get_texture(const std::string& name) const {
    const auto it = this->textures.find(name);
    if (it == this->textures.end()) {
      return nullptr;
    }
    return it->second;
  }

} // namespace engine::graphics::systems
