#pragma once
#include <memory>
#include <map>
#include <entt/core/type_info.hpp>
#include <type_traits>
#include "log.hpp"

namespace engine
{
  class ServiceRegistry
  {
  public:
    template <class Interface>
    std::shared_ptr<Interface> get_service();

    template <class Interface>
    void add_service(const std::shared_ptr<Interface> &service);

    template <class Interface, class Impl>
    void add_service(const std::shared_ptr<Impl> &service);

    template <class Interface>
    std::shared_ptr<Interface> remove_service();

  private:
    std::map<entt::type_info, std::shared_ptr<void>> services;

    void add_service(const entt::type_info &interface, std::shared_ptr<void> &&service);
    std::shared_ptr<void> get_service(const entt::type_info &interface);
    std::shared_ptr<void> remove_service(const entt::type_info &interface);
  };

  // //////////////////////////// //
  // /////////// IMPL /////////// //
  // //////////////////////////// //

  template <class Interface>
  std::shared_ptr<Interface> ServiceRegistry::get_service()
  {
    return std::reinterpret_pointer_cast<Interface>(get_service(entt::type_id<Interface>()));
  }

  template <class Interface>
  void ServiceRegistry::add_service(const std::shared_ptr<Interface> &service)
  {
    std::shared_ptr<void> void_service = std::reinterpret_pointer_cast<void>(service);
    add_service(entt::type_id<Interface>(), std::move(void_service));
  }

  template <class Interface, class Impl>
  void ServiceRegistry::add_service(const std::shared_ptr<Impl> &service)
  {
    static_assert(std::is_base_of_v<Interface, Impl>, "Interface is not base of implementation");
#ifdef __cpp_lib_is_virtual_base_of
    static_assert(std::is_virtual_base_of_v<Interface, Impl>, "Interface is not virtual base of implementation");
#endif
    std::shared_ptr<void> void_service =
      std::reinterpret_pointer_cast<void>(std::static_pointer_cast<Interface>(service));
    add_service(entt::type_id<Interface>(), std::move(void_service));
  }

  template <class Interface>
  std::shared_ptr<Interface> ServiceRegistry::remove_service()
  {
    return std::reinterpret_pointer_cast<Interface>(remove_service(entt::type_id<Interface>()));
  }


} // namespace core
