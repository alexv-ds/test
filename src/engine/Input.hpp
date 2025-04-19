#pragma once
#include <array>
#include <string_view>

namespace engine {

  class Input {
  public:
    enum class KeyCode {
      invalid = 0,
      space,
      apostrophe,
      comma,
      minus,
      period,
      slash,
      num_0,
      num_1,
      num_2,
      num_3,
      num_4,
      num_5,
      num_6,
      num_7,
      num_8,
      num_9,
      semicolon,
      equal,
      a,
      b,
      c,
      d,
      e,
      f,
      g,
      h,
      i,
      j,
      k,
      l,
      m,
      n,
      o,
      p,
      q,
      r,
      s,
      t,
      u,
      v,
      w,
      x,
      y,
      z,
      left_bracket,
      backslash,
      right_bracket,
      grave_accent,
      world_1,
      world_2,
      escape,
      enter,
      tab,
      backspace,
      insert,
      del,
      right,
      left,
      down,
      up,
      page_up,
      page_down,
      home,
      end,
      caps_lock,
      scroll_lock,
      num_lock,
      print_screen,
      pause,
      f1,
      f2,
      f3,
      f4,
      f5,
      f6,
      f7,
      f8,
      f9,
      f10,
      f11,
      f12,
      f13,
      f14,
      f15,
      f16,
      f17,
      f18,
      f19,
      f20,
      f21,
      f22,
      f23,
      f24,
      f25,
      kp_0,
      kp_1,
      kp_2,
      kp_3,
      kp_4,
      kp_5,
      kp_6,
      kp_7,
      kp_8,
      kp_9,
      kp_decimal,
      kp_divide,
      kp_multiply,
      kp_subtract,
      kp_add,
      kp_enter,
      kp_equal,
      left_shift,
      left_control,
      left_alt,
      left_super,
      right_shift,
      right_control,
      right_alt,
      right_super,
      menu,

      count_ // total count
    };

    struct KeyState {
      bool down = false;
      bool shift = false;
      bool ctrl = false;
      bool alt = false;
      bool super = false;
      operator bool() const noexcept { // NOLINT(*-explicit-constructor)
        return this->down;
      }
    };

    [[nodiscard]] const KeyState& keyboard(KeyCode key_code) const noexcept {
      return this->keyboard_state[static_cast<std::size_t>(key_code)];
    }

    virtual ~Input() = default;
    static std::string_view keycode_to_name(KeyCode code) noexcept;
    static KeyCode name_to_keycode(std::string_view name) noexcept;

  protected:
    using Keyboard = std::array<KeyState, static_cast<std::size_t>(KeyCode::count_)>;

    Keyboard keyboard_state{};
  };

} // namespace engine
