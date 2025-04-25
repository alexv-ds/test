#pragma once
#include <string>

namespace engine::sound::components {

  struct Source {
    std::string resource;
    bool loop = false;
  };

  struct Paused {};

  struct Stopped {};

};

