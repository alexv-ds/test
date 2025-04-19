#include "Input.hpp"

#include <map>
#include <unordered_map>

namespace engine {

  using KeyCode = Input::KeyCode;
  constexpr std::size_t g_keys_count_ = static_cast<std::size_t>(KeyCode::count_);

  constexpr static std::array<std::string_view, g_keys_count_> g_name_mapping = [] {
    std::array<std::string_view, g_keys_count_> arr{};
    auto idx = [](KeyCode key) { return static_cast<std::size_t>(key); };
    arr[idx(KeyCode::invalid)] = "INVALID";
    arr[idx(KeyCode::space)] = "SPACE";
    arr[idx(KeyCode::apostrophe)] = "APOSTROPHE";
    arr[idx(KeyCode::comma)] = "COMMA";
    arr[idx(KeyCode::minus)] = "MINUS";
    arr[idx(KeyCode::period)] = "PERIOD";
    arr[idx(KeyCode::slash)] = "SLASH";
    arr[idx(KeyCode::num_0)] = "NUM_0";
    arr[idx(KeyCode::num_1)] = "NUM_1";
    arr[idx(KeyCode::num_2)] = "NUM_2";
    arr[idx(KeyCode::num_3)] = "NUM_3";
    arr[idx(KeyCode::num_4)] = "NUM_4";
    arr[idx(KeyCode::num_5)] = "NUM_5";
    arr[idx(KeyCode::num_6)] = "NUM_6";
    arr[idx(KeyCode::num_7)] = "NUM_7";
    arr[idx(KeyCode::num_8)] = "NUM_8";
    arr[idx(KeyCode::num_9)] = "NUM_9";
    arr[idx(KeyCode::semicolon)] = "SEMICOLON";
    arr[idx(KeyCode::equal)] = "EQUAL";
    arr[idx(KeyCode::a)] = "A";
    arr[idx(KeyCode::b)] = "B";
    arr[idx(KeyCode::c)] = "C";
    arr[idx(KeyCode::d)] = "D";
    arr[idx(KeyCode::e)] = "E";
    arr[idx(KeyCode::f)] = "F";
    arr[idx(KeyCode::g)] = "G";
    arr[idx(KeyCode::h)] = "H";
    arr[idx(KeyCode::i)] = "I";
    arr[idx(KeyCode::j)] = "J";
    arr[idx(KeyCode::k)] = "K";
    arr[idx(KeyCode::l)] = "L";
    arr[idx(KeyCode::m)] = "M";
    arr[idx(KeyCode::n)] = "N";
    arr[idx(KeyCode::o)] = "O";
    arr[idx(KeyCode::p)] = "P";
    arr[idx(KeyCode::q)] = "Q";
    arr[idx(KeyCode::r)] = "R";
    arr[idx(KeyCode::s)] = "S";
    arr[idx(KeyCode::t)] = "T";
    arr[idx(KeyCode::u)] = "U";
    arr[idx(KeyCode::v)] = "V";
    arr[idx(KeyCode::w)] = "W";
    arr[idx(KeyCode::x)] = "X";
    arr[idx(KeyCode::y)] = "Y";
    arr[idx(KeyCode::z)] = "Z";
    arr[idx(KeyCode::left_bracket)] = "LEFT_BRACKET";
    arr[idx(KeyCode::backslash)] = "BACKSLASH";
    arr[idx(KeyCode::right_bracket)] = "RIGHT_BRACKET";
    arr[idx(KeyCode::grave_accent)] = "GRAVE_ACCENT";
    arr[idx(KeyCode::world_1)] = "WORLD_1";
    arr[idx(KeyCode::world_2)] = "WORLD_2";
    arr[idx(KeyCode::escape)] = "ESCAPE";
    arr[idx(KeyCode::enter)] = "ENTER";
    arr[idx(KeyCode::tab)] = "TAB";
    arr[idx(KeyCode::backspace)] = "BACKSPACE";
    arr[idx(KeyCode::insert)] = "INSERT";
    arr[idx(KeyCode::del)] = "DEL";
    arr[idx(KeyCode::right)] = "RIGHT";
    arr[idx(KeyCode::left)] = "LEFT";
    arr[idx(KeyCode::down)] = "DOWN";
    arr[idx(KeyCode::up)] = "UP";
    arr[idx(KeyCode::page_up)] = "PAGE_UP";
    arr[idx(KeyCode::page_down)] = "PAGE_DOWN";
    arr[idx(KeyCode::home)] = "HOME";
    arr[idx(KeyCode::end)] = "END";
    arr[idx(KeyCode::caps_lock)] = "CAPS_LOCK";
    arr[idx(KeyCode::scroll_lock)] = "SCROLL_LOCK";
    arr[idx(KeyCode::num_lock)] = "NUM_LOCK";
    arr[idx(KeyCode::print_screen)] = "PRINT_SCREEN";
    arr[idx(KeyCode::pause)] = "PAUSE";
    arr[idx(KeyCode::f1)] = "F1";
    arr[idx(KeyCode::f2)] = "F2";
    arr[idx(KeyCode::f3)] = "F3";
    arr[idx(KeyCode::f4)] = "F4";
    arr[idx(KeyCode::f5)] = "F5";
    arr[idx(KeyCode::f6)] = "F6";
    arr[idx(KeyCode::f7)] = "F7";
    arr[idx(KeyCode::f8)] = "F8";
    arr[idx(KeyCode::f9)] = "F9";
    arr[idx(KeyCode::f10)] = "F10";
    arr[idx(KeyCode::f11)] = "F11";
    arr[idx(KeyCode::f12)] = "F12";
    arr[idx(KeyCode::f13)] = "F13";
    arr[idx(KeyCode::f14)] = "F14";
    arr[idx(KeyCode::f15)] = "F15";
    arr[idx(KeyCode::f16)] = "F16";
    arr[idx(KeyCode::f17)] = "F17";
    arr[idx(KeyCode::f18)] = "F18";
    arr[idx(KeyCode::f19)] = "F19";
    arr[idx(KeyCode::f20)] = "F20";
    arr[idx(KeyCode::f21)] = "F21";
    arr[idx(KeyCode::f22)] = "F22";
    arr[idx(KeyCode::f23)] = "F23";
    arr[idx(KeyCode::f24)] = "F24";
    arr[idx(KeyCode::f25)] = "F25";
    arr[idx(KeyCode::kp_0)] = "KP_0";
    arr[idx(KeyCode::kp_1)] = "KP_1";
    arr[idx(KeyCode::kp_2)] = "KP_2";
    arr[idx(KeyCode::kp_3)] = "KP_3";
    arr[idx(KeyCode::kp_4)] = "KP_4";
    arr[idx(KeyCode::kp_5)] = "KP_5";
    arr[idx(KeyCode::kp_6)] = "KP_6";
    arr[idx(KeyCode::kp_7)] = "KP_7";
    arr[idx(KeyCode::kp_8)] = "KP_8";
    arr[idx(KeyCode::kp_9)] = "KP_9";
    arr[idx(KeyCode::kp_decimal)] = "KP_DECIMAL";
    arr[idx(KeyCode::kp_divide)] = "KP_DIVIDE";
    arr[idx(KeyCode::kp_multiply)] = "KP_MULTIPLY";
    arr[idx(KeyCode::kp_subtract)] = "KP_SUBTRACT";
    arr[idx(KeyCode::kp_add)] = "KP_ADD";
    arr[idx(KeyCode::kp_enter)] = "KP_ENTER";
    arr[idx(KeyCode::kp_equal)] = "KP_EQUAL";
    arr[idx(KeyCode::left_shift)] = "LEFT_SHIFT";
    arr[idx(KeyCode::left_control)] = "LEFT_CONTROL";
    arr[idx(KeyCode::left_alt)] = "LEFT_ALT";
    arr[idx(KeyCode::left_super)] = "LEFT_SUPER";
    arr[idx(KeyCode::right_shift)] = "RIGHT_SHIFT";
    arr[idx(KeyCode::right_control)] = "RIGHT_CONTROL";
    arr[idx(KeyCode::right_alt)] = "RIGHT_ALT";
    arr[idx(KeyCode::right_super)] = "RIGHT_SUPER";
    arr[idx(KeyCode::menu)] = "MENU";
    return arr;
  }();

  std::string_view Input::keycode_to_name(KeyCode code) noexcept {
    const auto idx = static_cast<std::size_t>(code);
    if (idx >= g_name_mapping.size()) {
      return g_name_mapping[static_cast<std::size_t>(KeyCode::invalid)];
    }
    return g_name_mapping[idx];
  }

  KeyCode Input::name_to_keycode(std::string_view name) noexcept {
    static std::map<std::string_view, KeyCode> map = {
      {"INVALID", KeyCode::invalid},
      {"SPACE", KeyCode::space},
      {"APOSTROPHE", KeyCode::apostrophe},
      {"COMMA", KeyCode::comma},
      {"MINUS", KeyCode::minus},
      {"PERIOD", KeyCode::period},
      {"SLASH", KeyCode::slash},
      {"NUM_0", KeyCode::num_0},
      {"NUM_1", KeyCode::num_1},
      {"NUM_2", KeyCode::num_2},
      {"NUM_3", KeyCode::num_3},
      {"NUM_4", KeyCode::num_4},
      {"NUM_5", KeyCode::num_5},
      {"NUM_6", KeyCode::num_6},
      {"NUM_7", KeyCode::num_7},
      {"NUM_8", KeyCode::num_8},
      {"NUM_9", KeyCode::num_9},
      {"SEMICOLON", KeyCode::semicolon},
      {"EQUAL", KeyCode::equal},
      {"A", KeyCode::a},
      {"B", KeyCode::b},
      {"C", KeyCode::c},
      {"D", KeyCode::d},
      {"E", KeyCode::e},
      {"F", KeyCode::f},
      {"G", KeyCode::g},
      {"H", KeyCode::h},
      {"I", KeyCode::i},
      {"J", KeyCode::j},
      {"K", KeyCode::k},
      {"L", KeyCode::l},
      {"M", KeyCode::m},
      {"N", KeyCode::n},
      {"O", KeyCode::o},
      {"P", KeyCode::p},
      {"Q", KeyCode::q},
      {"R", KeyCode::r},
      {"S", KeyCode::s},
      {"T", KeyCode::t},
      {"U", KeyCode::u},
      {"V", KeyCode::v},
      {"W", KeyCode::w},
      {"X", KeyCode::x},
      {"Y", KeyCode::y},
      {"Z", KeyCode::z},
      {"LEFT_BRACKET", KeyCode::left_bracket},
      {"BACKSLASH", KeyCode::backslash},
      {"RIGHT_BRACKET", KeyCode::right_bracket},
      {"GRAVE_ACCENT", KeyCode::grave_accent},
      {"WORLD_1", KeyCode::world_1},
      {"WORLD_2", KeyCode::world_2},
      {"ESCAPE", KeyCode::escape},
      {"ENTER", KeyCode::enter},
      {"TAB", KeyCode::tab},
      {"BACKSPACE", KeyCode::backspace},
      {"INSERT", KeyCode::insert},
      {"DEL", KeyCode::del},
      {"RIGHT", KeyCode::right},
      {"LEFT", KeyCode::left},
      {"DOWN", KeyCode::down},
      {"UP", KeyCode::up},
      {"PAGE_UP", KeyCode::page_up},
      {"PAGE_DOWN", KeyCode::page_down},
      {"HOME", KeyCode::home},
      {"END", KeyCode::end},
      {"CAPS_LOCK", KeyCode::caps_lock},
      {"SCROLL_LOCK", KeyCode::scroll_lock},
      {"NUM_LOCK", KeyCode::num_lock},
      {"PRINT_SCREEN", KeyCode::print_screen},
      {"PAUSE", KeyCode::pause},
      {"F1", KeyCode::f1},
      {"F2", KeyCode::f2},
      {"F3", KeyCode::f3},
      {"F4", KeyCode::f4},
      {"F5", KeyCode::f5},
      {"F6", KeyCode::f6},
      {"F7", KeyCode::f7},
      {"F8", KeyCode::f8},
      {"F9", KeyCode::f9},
      {"F10", KeyCode::f10},
      {"F11", KeyCode::f11},
      {"F12", KeyCode::f12},
      {"F13", KeyCode::f13},
      {"F14", KeyCode::f14},
      {"F15", KeyCode::f15},
      {"F16", KeyCode::f16},
      {"F17", KeyCode::f17},
      {"F18", KeyCode::f18},
      {"F19", KeyCode::f19},
      {"F20", KeyCode::f20},
      {"F21", KeyCode::f21},
      {"F22", KeyCode::f22},
      {"F23", KeyCode::f23},
      {"F24", KeyCode::f24},
      {"F25", KeyCode::f25},
      {"KP_0", KeyCode::kp_0},
      {"KP_1", KeyCode::kp_1},
      {"KP_2", KeyCode::kp_2},
      {"KP_3", KeyCode::kp_3},
      {"KP_4", KeyCode::kp_4},
      {"KP_5", KeyCode::kp_5},
      {"KP_6", KeyCode::kp_6},
      {"KP_7", KeyCode::kp_7},
      {"KP_8", KeyCode::kp_8},
      {"KP_9", KeyCode::kp_9},
      {"KP_DECIMAL", KeyCode::kp_decimal},
      {"KP_DIVIDE", KeyCode::kp_divide},
      {"KP_MULTIPLY", KeyCode::kp_multiply},
      {"KP_SUBTRACT", KeyCode::kp_subtract},
      {"KP_ADD", KeyCode::kp_add},
      {"KP_ENTER", KeyCode::kp_enter},
      {"KP_EQUAL", KeyCode::kp_equal},
      {"LEFT_SHIFT", KeyCode::left_shift},
      {"LEFT_CONTROL", KeyCode::left_control},
      {"LEFT_ALT", KeyCode::left_alt},
      {"LEFT_SUPER", KeyCode::left_super},
      {"RIGHT_SHIFT", KeyCode::right_shift},
      {"RIGHT_CONTROL", KeyCode::right_control},
      {"RIGHT_ALT", KeyCode::right_alt},
      {"RIGHT_SUPER", KeyCode::right_super},
      {"MENU", KeyCode::menu},
    };

    const auto it = map.find(name);
    if (it == map.end()) {
      return KeyCode::invalid;
    }
    return it->second;
  }
} // namespace engine
