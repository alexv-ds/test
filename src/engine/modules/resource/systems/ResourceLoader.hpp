#pragma once
#include <engine/System.hpp>

namespace engine::resource::systems {

  class ResourceLoader final : public System {
  public:
    ResourceLoader();
    ~ResourceLoader();
    void start() override;
    void stop() override;
    void update() override;
  };

} // namespace engine::resource::systems
