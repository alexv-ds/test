#pragma once
#include <engine/Input.hpp>
#include <sokol_app.h>

class InputImpl final : public engine::Input {
public:
  void keyboard_handle_event(const sapp_event* event);
  void keyboard_clean();
};



