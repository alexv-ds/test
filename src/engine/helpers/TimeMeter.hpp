#pragma once
#include <chrono>

namespace engine::helpers {

  class TimeMeter {
  public:
    using clock = std::chrono::high_resolution_clock;
    using time_point = clock::time_point;
    using duration = clock::duration;

    TimeMeter() noexcept : start_point(clock::now()) {}

    template <class Duration = clock::duration>
    Duration measure() const noexcept {
      const time_point stop_point = clock::now();
      return std::chrono::duration_cast<Duration>(stop_point - this->start_point);
    }

  private:
    const time_point start_point;
  };

} // namespace engine::helpers
