#include "InputImpl.hpp"
#include <engine/log.hpp>
#include <array>

using KeyCode = engine::Input::KeyCode;
constexpr std::size_t sokol_keys_count = 349; // see sapp_keycode into sokol_app.h

constexpr static std::array<KeyCode, sokol_keys_count> g_sokol_to_engine = [] {
  std::array<KeyCode, sokol_keys_count> arr{};
  auto idx = [](const sapp_keycode code) {
    if (static_cast<std::size_t>(code) >= sokol_keys_count) {
      throw std::out_of_range("KeyCode out of range");
    }
    return static_cast<std::size_t>(code);
  };
  arr[idx(SAPP_KEYCODE_INVALID)] = KeyCode::invalid;
  arr[idx(SAPP_KEYCODE_SPACE)] = KeyCode::space;
  arr[idx(SAPP_KEYCODE_APOSTROPHE)] = KeyCode::apostrophe;
  arr[idx(SAPP_KEYCODE_COMMA)] = KeyCode::comma;
  arr[idx(SAPP_KEYCODE_MINUS)] = KeyCode::minus;
  arr[idx(SAPP_KEYCODE_PERIOD)] = KeyCode::period;
  arr[idx(SAPP_KEYCODE_SLASH)] = KeyCode::slash;
  arr[idx(SAPP_KEYCODE_0)] = KeyCode::num_0;
  arr[idx(SAPP_KEYCODE_1)] = KeyCode::num_1;
  arr[idx(SAPP_KEYCODE_2)] = KeyCode::num_2;
  arr[idx(SAPP_KEYCODE_3)] = KeyCode::num_3;
  arr[idx(SAPP_KEYCODE_4)] = KeyCode::num_4;
  arr[idx(SAPP_KEYCODE_5)] = KeyCode::num_5;
  arr[idx(SAPP_KEYCODE_6)] = KeyCode::num_6;
  arr[idx(SAPP_KEYCODE_7)] = KeyCode::num_7;
  arr[idx(SAPP_KEYCODE_8)] = KeyCode::num_8;
  arr[idx(SAPP_KEYCODE_9)] = KeyCode::num_9;
  arr[idx(SAPP_KEYCODE_SEMICOLON)] = KeyCode::semicolon;
  arr[idx(SAPP_KEYCODE_EQUAL)] = KeyCode::equal;
  arr[idx(SAPP_KEYCODE_A)] = KeyCode::a;
  arr[idx(SAPP_KEYCODE_B)] = KeyCode::b;
  arr[idx(SAPP_KEYCODE_C)] = KeyCode::c;
  arr[idx(SAPP_KEYCODE_D)] = KeyCode::d;
  arr[idx(SAPP_KEYCODE_E)] = KeyCode::e;
  arr[idx(SAPP_KEYCODE_F)] = KeyCode::f;
  arr[idx(SAPP_KEYCODE_G)] = KeyCode::g;
  arr[idx(SAPP_KEYCODE_H)] = KeyCode::h;
  arr[idx(SAPP_KEYCODE_I)] = KeyCode::i;
  arr[idx(SAPP_KEYCODE_J)] = KeyCode::j;
  arr[idx(SAPP_KEYCODE_K)] = KeyCode::k;
  arr[idx(SAPP_KEYCODE_L)] = KeyCode::l;
  arr[idx(SAPP_KEYCODE_M)] = KeyCode::m;
  arr[idx(SAPP_KEYCODE_N)] = KeyCode::n;
  arr[idx(SAPP_KEYCODE_O)] = KeyCode::o;
  arr[idx(SAPP_KEYCODE_P)] = KeyCode::p;
  arr[idx(SAPP_KEYCODE_Q)] = KeyCode::q;
  arr[idx(SAPP_KEYCODE_R)] = KeyCode::r;
  arr[idx(SAPP_KEYCODE_S)] = KeyCode::s;
  arr[idx(SAPP_KEYCODE_T)] = KeyCode::t;
  arr[idx(SAPP_KEYCODE_U)] = KeyCode::u;
  arr[idx(SAPP_KEYCODE_V)] = KeyCode::v;
  arr[idx(SAPP_KEYCODE_W)] = KeyCode::w;
  arr[idx(SAPP_KEYCODE_X)] = KeyCode::x;
  arr[idx(SAPP_KEYCODE_Y)] = KeyCode::y;
  arr[idx(SAPP_KEYCODE_Z)] = KeyCode::z;
  arr[idx(SAPP_KEYCODE_LEFT_BRACKET)] = KeyCode::left_bracket;
  arr[idx(SAPP_KEYCODE_BACKSLASH)] = KeyCode::backslash;
  arr[idx(SAPP_KEYCODE_RIGHT_BRACKET)] = KeyCode::right_bracket;
  arr[idx(SAPP_KEYCODE_GRAVE_ACCENT)] = KeyCode::grave_accent;
  arr[idx(SAPP_KEYCODE_WORLD_1)] = KeyCode::invalid;
  arr[idx(SAPP_KEYCODE_WORLD_2)] = KeyCode::invalid;
  arr[idx(SAPP_KEYCODE_ESCAPE)] = KeyCode::escape;
  arr[idx(SAPP_KEYCODE_ENTER)] = KeyCode::enter;
  arr[idx(SAPP_KEYCODE_TAB)] = KeyCode::tab;
  arr[idx(SAPP_KEYCODE_BACKSPACE)] = KeyCode::backspace;
  arr[idx(SAPP_KEYCODE_INSERT)] = KeyCode::insert;
  arr[idx(SAPP_KEYCODE_DELETE)] = KeyCode::del;
  arr[idx(SAPP_KEYCODE_RIGHT)] = KeyCode::right;
  arr[idx(SAPP_KEYCODE_LEFT)] = KeyCode::left;
  arr[idx(SAPP_KEYCODE_DOWN)] = KeyCode::down;
  arr[idx(SAPP_KEYCODE_UP)] = KeyCode::up;
  arr[idx(SAPP_KEYCODE_PAGE_UP)] = KeyCode::page_up;
  arr[idx(SAPP_KEYCODE_PAGE_DOWN)] = KeyCode::page_down;
  arr[idx(SAPP_KEYCODE_HOME)] = KeyCode::home;
  arr[idx(SAPP_KEYCODE_END)] = KeyCode::end;
  arr[idx(SAPP_KEYCODE_CAPS_LOCK)] = KeyCode::caps_lock;
  arr[idx(SAPP_KEYCODE_SCROLL_LOCK)] = KeyCode::scroll_lock;
  arr[idx(SAPP_KEYCODE_NUM_LOCK)] = KeyCode::num_lock;
  arr[idx(SAPP_KEYCODE_PRINT_SCREEN)] = KeyCode::print_screen;
  arr[idx(SAPP_KEYCODE_PAUSE)] = KeyCode::pause;
  arr[idx(SAPP_KEYCODE_F1)] = KeyCode::f1;
  arr[idx(SAPP_KEYCODE_F2)] = KeyCode::f2;
  arr[idx(SAPP_KEYCODE_F3)] = KeyCode::f3;
  arr[idx(SAPP_KEYCODE_F4)] = KeyCode::f4;
  arr[idx(SAPP_KEYCODE_F5)] = KeyCode::f5;
  arr[idx(SAPP_KEYCODE_F6)] = KeyCode::f6;
  arr[idx(SAPP_KEYCODE_F7)] = KeyCode::f7;
  arr[idx(SAPP_KEYCODE_F8)] = KeyCode::f8;
  arr[idx(SAPP_KEYCODE_F9)] = KeyCode::f9;
  arr[idx(SAPP_KEYCODE_F10)] = KeyCode::f10;
  arr[idx(SAPP_KEYCODE_F11)] = KeyCode::f11;
  arr[idx(SAPP_KEYCODE_F12)] = KeyCode::f12;
  arr[idx(SAPP_KEYCODE_F13)] = KeyCode::f13;
  arr[idx(SAPP_KEYCODE_F14)] = KeyCode::f14;
  arr[idx(SAPP_KEYCODE_F15)] = KeyCode::f15;
  arr[idx(SAPP_KEYCODE_F16)] = KeyCode::f16;
  arr[idx(SAPP_KEYCODE_F17)] = KeyCode::f17;
  arr[idx(SAPP_KEYCODE_F18)] = KeyCode::f18;
  arr[idx(SAPP_KEYCODE_F19)] = KeyCode::f19;
  arr[idx(SAPP_KEYCODE_F20)] = KeyCode::f20;
  arr[idx(SAPP_KEYCODE_F21)] = KeyCode::f21;
  arr[idx(SAPP_KEYCODE_F22)] = KeyCode::f22;
  arr[idx(SAPP_KEYCODE_F23)] = KeyCode::f23;
  arr[idx(SAPP_KEYCODE_F24)] = KeyCode::f24;
  arr[idx(SAPP_KEYCODE_F25)] = KeyCode::f25;
  arr[idx(SAPP_KEYCODE_KP_0)] = KeyCode::kp_0;
  arr[idx(SAPP_KEYCODE_KP_1)] = KeyCode::kp_1;
  arr[idx(SAPP_KEYCODE_KP_2)] = KeyCode::kp_2;
  arr[idx(SAPP_KEYCODE_KP_3)] = KeyCode::kp_3;
  arr[idx(SAPP_KEYCODE_KP_4)] = KeyCode::kp_4;
  arr[idx(SAPP_KEYCODE_KP_5)] = KeyCode::kp_5;
  arr[idx(SAPP_KEYCODE_KP_6)] = KeyCode::kp_6;
  arr[idx(SAPP_KEYCODE_KP_7)] = KeyCode::kp_7;
  arr[idx(SAPP_KEYCODE_KP_8)] = KeyCode::kp_8;
  arr[idx(SAPP_KEYCODE_KP_9)] = KeyCode::kp_9;
  arr[idx(SAPP_KEYCODE_KP_DECIMAL)] = KeyCode::kp_decimal;
  arr[idx(SAPP_KEYCODE_KP_DIVIDE)] = KeyCode::kp_divide;
  arr[idx(SAPP_KEYCODE_KP_MULTIPLY)] = KeyCode::kp_multiply;
  arr[idx(SAPP_KEYCODE_KP_SUBTRACT)] = KeyCode::kp_subtract;
  arr[idx(SAPP_KEYCODE_KP_ADD)] = KeyCode::kp_add;
  arr[idx(SAPP_KEYCODE_KP_ENTER)] = KeyCode::kp_enter;
  arr[idx(SAPP_KEYCODE_KP_EQUAL)] = KeyCode::kp_equal;
  arr[idx(SAPP_KEYCODE_LEFT_SHIFT)] = KeyCode::left_shift;
  arr[idx(SAPP_KEYCODE_LEFT_CONTROL)] = KeyCode::left_control;
  arr[idx(SAPP_KEYCODE_LEFT_ALT)] = KeyCode::left_alt;
  arr[idx(SAPP_KEYCODE_LEFT_SUPER)] = KeyCode::left_super;
  arr[idx(SAPP_KEYCODE_RIGHT_SHIFT)] = KeyCode::right_shift;
  arr[idx(SAPP_KEYCODE_RIGHT_CONTROL)] = KeyCode::right_control;
  arr[idx(SAPP_KEYCODE_RIGHT_ALT)] = KeyCode::right_alt;
  arr[idx(SAPP_KEYCODE_RIGHT_SUPER)] = KeyCode::right_super;
  arr[idx(SAPP_KEYCODE_MENU)] = KeyCode::menu;
  return arr;
}();

void InputImpl::handle_event(const sapp_event* event) {
  if (event->type != SAPP_EVENTTYPE_KEY_DOWN && event->type != SAPP_EVENTTYPE_KEY_UP) {
    return;
  }

  const auto sokol_idx = static_cast<std::size_t>(event->key_code);
  if (sokol_idx >= sokol_keys_count) {
    LOG_WARN("cannot handle key code - {}", sokol_idx);
    return;
  }

  KeyState& state =
    this->keyboard_state[static_cast<std::size_t>(g_sokol_to_engine[sokol_idx])];
  if (event->type == SAPP_EVENTTYPE_KEY_UP) {
    state = KeyState{};
  } else {
    state.down = true;
    state.shift = event->modifiers & SAPP_MODIFIER_SHIFT;
    state.ctrl = event->modifiers & SAPP_MODIFIER_CTRL;
    state.alt = event->modifiers & SAPP_MODIFIER_ALT;
    state.super = event->modifiers & SAPP_MODIFIER_SUPER;
  }
}
