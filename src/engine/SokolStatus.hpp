#pragma once

namespace engine {

  class SokolStatus {
  public:
    virtual ~SokolStatus() = default;
    virtual bool is_sokol_initialized() const noexcept = 0;
    virtual bool is_gfx_initialized() const noexcept = 0;
    virtual bool is_spg_initialized() const noexcept = 0;
  };

} // namespace engine
