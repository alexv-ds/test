#pragma once
#include <engine/SokolStatus.hpp>

class SokolStatusImpl final : public engine::SokolStatus {
public:
  explicit SokolStatusImpl(bool* const imgui_event_interception_flag_) :
      imgui_event_interception_flag(imgui_event_interception_flag_) {}

  bool is_sokol_initialized() const noexcept override { return true; }
  bool is_gfx_initialized() const noexcept override { return true; }
  bool is_spg_initialized() const noexcept override { return true; }
  bool is_util_imgui_initialized() const noexcept override { return true; }
  void set_imgui_events_interception(const bool enable) const noexcept override {
    *imgui_event_interception_flag = enable;
  }
private:
  bool* imgui_event_interception_flag;
};


