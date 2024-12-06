#include <fstream>
#include <filesystem>
#include <format>
#include <yaml-cpp/yaml.h>
#include "SystemScheduler.hpp"

#include <iostream>

namespace engine
{

  SystemScheduler::SystemScheduler(const std::filesystem::path &config_path)
  {
    this->load_orders(config_path);
  }

  void SystemScheduler::load_orders(const std::filesystem::path &config_path)
  {
    if (!exists(config_path))
    {
      throw std::runtime_error(std::format("File '{}' does not exist", config_path.string()));
    }

    if (!is_regular_file(config_path))
    {
      throw std::runtime_error(std::format("File '{}' is not regular file", config_path.string()));
    }

    YAML::Node doc = YAML::LoadFile(config_path.string());
    // for (auto ) {}


    // auto keks = 123;
  }


}
