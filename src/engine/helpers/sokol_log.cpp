#include "sokol_log.hpp"

namespace engine::helpers {

  void sokol_log(const char* tag, std::uint32_t log_level, const std::uint32_t,
                 const char* message_or_null, const std::uint32_t line_nr,
                 const char* filename_or_null, void*) {
    if (!message_or_null) {
      return;
    }

    const spdlog::level::level_enum level = [log_level] {
      switch (log_level) {
      case 0:
        return spdlog::level::critical;
      case 1:
        return spdlog::level::err;
      case 3:
        return spdlog::level::info;
      default:
        return spdlog::level::warn;
      }
    }();

    const spdlog::source_loc loc{filename_or_null, static_cast<int>(line_nr), nullptr};

    if (tag) {
      spdlog::default_logger()->log(loc, level, "[{}] {}", tag, message_or_null);
    } else {
      spdlog::default_logger()->log(loc, level, message_or_null);
    }
  }

} // namespace engine::helpers
