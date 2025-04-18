#pragma once
#include <entt/core/type_info.hpp>

#include <memory>

namespace engine {

  class EventEmitter : public std::enable_shared_from_this<EventEmitter> {
  public:

    virtual ~EventEmitter() = default;

  private:
//    std::map<entt::id_type, std::>

  };


} // namespace engine
