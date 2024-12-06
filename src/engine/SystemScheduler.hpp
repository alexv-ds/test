#pragma once
#include <filesystem>
#include <vector>
#include "System.hpp"


namespace engine
{
  class SystemScheduler
  {
  public:
    explicit SystemScheduler(const std::filesystem::path &config_path);

    void add_system(std::shared_ptr<System> system);

  private:
    void load_orders(const std::filesystem::path &config_path);

    std::vector<std::string> order;
    std::vector<std::shared_ptr<System>> systems;
  };

  // //////////////////////////// //
  // /////////// IMPL /////////// //
  // //////////////////////////// //

  inline void SystemScheduler::add_system(std::shared_ptr<System> system)
  {
    systems.push_back(std::move(system));
  }
}
