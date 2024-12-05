#include <type_traits>
#include <stdexcept>
#include <format>
#include "EngineLifecycle.hpp"
#include "../log.hpp"


namespace engine::service
{
  void EngineLifecycle::add_callback_static(Stage stage, callback cb)
  {
    using stage_underlying_t = std::underlying_type_t<Stage>;
    static_assert(std::is_same_v<std::size_t, stage_underlying_t>);
    const auto index = static_cast<std::size_t>(stage);
    if (index >= this->callbacks.size())
    {
      throw std::out_of_range(std::format("Cannot add callback - stage ({}) >= {}", index, this->callbacks.size()));
    }
    this->callbacks[index].push_back(std::move(cb));
  }

  void EngineLifecycle::emit(Engine &engine, Stage stage)
  {
    using stage_underlying_t = std::underlying_type_t<Stage>;
    static_assert(std::is_same_v<std::size_t, stage_underlying_t>);
    const auto index = static_cast<std::size_t>(stage);
    if (index >= this->callbacks.size())
    {
      throw std::out_of_range(std::format("Cannot emit stage - stage ({}) >= {}", index, this->callbacks.size()));
    }
    this->tmp_current_execute.clear();
    std::ranges::copy(this->callbacks[index], std::back_inserter(this->tmp_current_execute));
    for (const auto &cb : this->tmp_current_execute)
    {
      try
      {
        cb(engine);
      }
      catch (std::exception &e)
      {
        ENGINE_ERROR("Exception thrown in EngineLifecycle::emit (Stage - {}): {}", index, e.what());
      }
    }
  }

}
