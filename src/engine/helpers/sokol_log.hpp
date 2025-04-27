#pragma once
#include <cstdint>
#include <spdlog/spdlog.h>

namespace engine::helpers {

  /**
   * Helper function for redirect sokol log to SPDLOG
   *
   * @param tag sokol log tag
   * @param log_level 0=panic, 1=error, 2=warning, 3=info
   * @param log_item_id SFETCH_LOGITEM_*
   * @param message_or_null a message string, may be nullptr in release mode
   * @param line_nr line number
   * @param filename_or_null source filename, may be nullptr in release mode
   * @param user_data (unused)
   */
  void sokol_log(const char* tag, std::uint32_t log_level, std::uint32_t log_item_id,
                 const char* message_or_null, std::uint32_t line_nr,
                 const char* filename_or_null, void* user_data);

} // namespace engine::helpers
