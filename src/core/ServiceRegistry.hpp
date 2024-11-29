#pragma once
#include <memory>


struct ServiceDescription {};

namespace core
{
  class ServiceRegistry
  {
  public:
    template <class T>
    std::shared_ptr<T> get() const;

    std::


  private:
    // struct Impl;
  };

} // namespace core
