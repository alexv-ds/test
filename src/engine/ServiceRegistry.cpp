//
// Created by alexv on 29.11.24.
//

#include "ServiceRegistry.hpp"
#include <format>
#include <stdexcept>

namespace engine
{
  void ServiceRegistry::add_service(const entt::type_info &interface, std::shared_ptr<void> &&service)
  {
    if (const auto it = services.find(interface); it != services.end())
    {
      throw std::runtime_error(std::format("Service '{}' already registered", interface.name()));
    }
    services.emplace(interface, std::move(service));
    LOG_TRACE("Added service - {}", interface.name());
  }

  std::shared_ptr<void> ServiceRegistry::get_service(const entt::type_info &interface)
  {
    const auto it = services.find(interface);
    if (it == services.end())
    {
      throw std::runtime_error(std::format("Service '{}' does not exist", interface.name()));
    }
    return it->second;
  }

  std::shared_ptr<void> ServiceRegistry::remove_service(const entt::type_info &interface)
  {
    const auto it = services.find(interface);
    if (it == services.end())
    {
      return nullptr;
    }
    const std::shared_ptr service = std::move(it->second);
    services.erase(it);
    return service;
  }

} // core
