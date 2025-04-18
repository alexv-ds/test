#pragma once
#include <vector>
#include <string>
#include <entt/entity/entity.hpp>
#include "Map.hpp"
#include "noncopyable.hpp"

namespace engine {

  class InstanceImpl final : public Instance, public noncopyable {
  public:
    explicit InstanceImpl(std::string_view name, std::uint32_t id);
    std::string_view name() const override;
  private:
    const std::string instance_name;
  };

  class MapImpl final : public Map, public noncopyable {
  public:
    MapImpl();
    const Instance& create_instance(std::string_view name) override;
    void delete_instance(std::uint32_t id) override;
    [[nodiscard]] const Instance& get_instance(std::uint32_t id) const override;
    [[nodiscard]] const Instance* try_get_instance(std::uint32_t id) const noexcept override;

    
  private:
    std::vector<std::shared_ptr<InstanceImpl>> instances;
    std::vector<entt::entity> entities_for_delete;
  };

} // namespace engine
