#pragma once
#include <memory>
#include <vector>
#include <string>
#include <optional>

namespace engine::resource::components {

  struct Load {
    std::string name;
  };

  struct Resource {
    std::shared_ptr<std::vector<char>> resource;
  };

  struct Ready {
    std::optional<std::string> error;
  };

}