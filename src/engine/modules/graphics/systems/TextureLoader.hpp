#pragma once
#include <engine/System.hpp>
#include <engine/modules/resource/services/Loader.hpp>
#include <unordered_map>
#include <unordered_set>
#include "../components_private.hpp"

namespace engine::graphics::systems {

  struct LoadingResource {
    std::string name;
  };


  class TextureLoader final : public System {
  public:
    explicit TextureLoader(std::shared_ptr<resource::Loader> loader) :
        loader(std::move(loader)) {};

    void start() override;
    void stop() override;
    void update() override;

  private:
    std::shared_ptr<resource::Loader> loader;
    // // std::unordered_multimap<std::shared_ptr<resource::Resource>, entt::entity>
    // std::unordered_map<std::string, std::shared_ptr<resource::Resource>> resources;
    // // std::unordered_map<std::string, detail::components::Texture> textures;

    std::unordered_map<std::string, std::shared_ptr<sg_image>> textures;

    std::unordered_set<entt::entity> dirty_entities;

    void add_to_dirty(entt::entity);
    [[nodiscard]] std::shared_ptr<sg_image> get_texture(const std::string& name) const;
  };

} // namespace engine::graphics::systems
