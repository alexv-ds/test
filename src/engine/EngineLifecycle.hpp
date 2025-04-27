#pragma once
#include <functional>
#include <array>
#include <vector>
#include "ServiceRegistry.hpp"

namespace engine
{
  class EngineLifecycle
  {
  public:
    using Callback = std::function<void(ServiceRegistry &)>;
    using CallbackId = int;

    enum class Stage : std::size_t
    {
      init_pre = 0,
      init_post = 1,
      frame_pre = 2,
      system_run_pre = 3,
      system_run_post = 4,
      frame_post = 5,
      exit_pre = 6,
      exit_post = 7
    };

    void add_callback_static(Stage stage, Callback cb);
    [[nodiscard]] CallbackId add_callback(Stage stage, Callback cb);
    void remove_callback(CallbackId id);

    void emit(ServiceRegistry &registry, Stage stage);

  private:
    int callback_counter_ = 0;

    std::array<std::vector<Callback>, 8> callbacks{};
    std::array<std::map<int,Callback>, 8> dynamic_callbacks{};
    std::vector<Callback> tmp_current_execute;
  };

}
