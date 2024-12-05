#pragma once
#include <filesystem>

namespace engine
{
  class SystemScheduler
  {
  public:
    SystemScheduler(const std::filesystem::path& config_path);

  };
}
