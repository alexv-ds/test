#include "EngineLifecycle.hpp"
#include <format>
#include <ranges>
#include <stdexcept>
#include <type_traits>
#include "engine/ServiceRegistry.hpp"
#include "log.hpp"


namespace engine {
  void EngineLifecycle::add_callback_static(Stage stage, Callback cb) {
    using stage_underlying_t = std::underlying_type_t<Stage>;
    static_assert(std::is_same_v<std::size_t, stage_underlying_t>);
    const auto index = static_cast<std::size_t>(stage);
    if (index >= this->callbacks.size()) {
      throw std::out_of_range(std::format("cannot add callback: invalid stage ({} >= {})",
                                          index, this->callbacks.size()));
    }
    this->callbacks[index].push_back(std::move(cb));
  }

  EngineLifecycle::CallbackId EngineLifecycle::add_callback(Stage stage, Callback cb) {
    static_assert(std::is_same_v<std::size_t, std::underlying_type_t<Stage>>);
    const auto index = static_cast<std::size_t>(stage);
    if (index >= this->dynamic_callbacks.size()) {
      throw std::out_of_range(std::format("cannot add callback: invalid stage ({} >= {})",
                                          index, this->dynamic_callbacks.size()));
    }
    const auto id = ++this->callback_counter_;
    this->dynamic_callbacks[index].insert({id, std::move(cb)});
    return id;
  }

  void EngineLifecycle::remove_callback(const CallbackId id) {
    bool removed = false;
    for (auto& map : this->dynamic_callbacks) {
      if (const auto it = map.find(id); it != map.end()) {
        map.erase(it);
        removed = true;
      }
    }
    if (!removed) {
      throw std::invalid_argument(std::format("invalid callback id {}", id));
    }
  }

  void EngineLifecycle::emit(ServiceRegistry& registry, Stage stage) {
    using stage_underlying_t = std::underlying_type_t<Stage>;
    static_assert(std::is_same_v<std::size_t, stage_underlying_t>);
    const auto index = static_cast<std::size_t>(stage);
    if (index >= this->callbacks.size()) {
      throw std::out_of_range(std::format("Cannot emit stage - stage ({}) >= {}", index,
                                          this->callbacks.size()));
    }
    this->tmp_current_execute.clear();
    std::ranges::copy(this->callbacks[index],
                      std::back_inserter(this->tmp_current_execute));
    for (const auto& val : this->dynamic_callbacks[index] | std::views::values) {
      this->tmp_current_execute.push_back(val);
    }
    for (const auto& cb : this->tmp_current_execute) {
      try {
        cb(registry);
      }
      catch (std::exception& e) {
        LOG_ERROR("Exception thrown in EngineLifecycle::emit (Stage - {}): {}", index,
                  e.what());
      }
    }
  }

} // namespace engine
