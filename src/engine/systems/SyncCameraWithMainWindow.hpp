#pragma once
#include "../System.hpp"

namespace engine::systems {

class SyncCameraWithMainWindow final : public System {
public:
  void update() override;
};

}


