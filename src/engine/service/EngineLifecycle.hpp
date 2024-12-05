#pragma once
#include <functional>
#include <array>
#include <vector>

namespace engine
{
  class Engine;
}

namespace engine::service
{
  class EngineLifecycle
  {
  public:
    using callback = std::function<void(Engine &)>;

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

    void add_callback_static(Stage stage, callback cb);
    void emit(Engine &engine, Stage stage);

  private:
    std::array<std::vector<callback>, 8> callbacks;
    std::vector<callback> tmp_current_execute;
  };

}
