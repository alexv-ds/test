#pragma once

namespace engine {
  class noncopyable {
  public:
    noncopyable() = default;
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
  };
} // namespace engine
