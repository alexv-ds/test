#pragma once

#include <memory>
#include <flat_map>

namespace engine {

  class EventEmitter : public std::enable_shared_from_this<EventEmitter> {
  public:

    virtual ~EventEmitter();

  private:

  };

} // namespace engine
