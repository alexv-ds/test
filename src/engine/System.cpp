#include "System.hpp"

namespace engine
{
  void System::init(std::shared_ptr<entt::registry> registry)
  {
    if (this->registry_ != nullptr)
    {
      throw std::runtime_error("System already initialized");
    }
    this->registry_ = std::move(registry);
  }
}
