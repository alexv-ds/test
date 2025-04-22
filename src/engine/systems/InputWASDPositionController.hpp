#pragma once
#include <memory>
#include "../Input.hpp"
#include "../System.hpp"

namespace engine::systems {

  class InputWASDPositionController final : public System {
  public:
    explicit InputWASDPositionController(std::shared_ptr<Input> input) :
        input(std::move(input)) {}
    void update() override;
  private:
    std::shared_ptr<Input> input;
  };

} // namespace engine::systems
