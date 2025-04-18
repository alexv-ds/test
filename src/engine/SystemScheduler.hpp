#pragma once
#include <filesystem>
#include <functional>
#include <map>
#include <vector>
#include "System.hpp"

namespace engine {
  class SystemScheduler {
  public:
    explicit SystemScheduler(const std::filesystem::path &config_path, std::shared_ptr<entt::registry> ecs_registry);
    ~SystemScheduler();

    void add_system(std::string_view name, std::shared_ptr<System> system);
    void add_system(std::string_view name, std::function<void(entt::registry &)> system);
    void run_systems();

  private:
    void load_orders(const std::filesystem::path &config_path);

    void reindex_systems();

    std::vector<std::string> order;
    std::vector<std::pair<std::string, std::shared_ptr<System>>> systems;
    std::map<std::string, std::shared_ptr<System>> system_map;
    std::shared_ptr<entt::registry> ecs_registry;
    bool need_reindex = false;
  };
} // namespace engine
