#pragma once
#include <engine/SokolStatus.hpp>

class SokolStatusImpl final : public engine::SokolStatus {
public:
  bool is_sokol_initialized() const noexcept override { return true; }
  bool is_gfx_initialized() const noexcept override { return true; }
  bool is_spg_initialized() const noexcept override { return true; }
  bool is_util_imgui_initialized() const noexcept override { return true; }
};


