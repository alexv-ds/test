CPMAddPackage(
  NAME spdlog
  URL "https://github.com/gabime/spdlog/archive/refs/tags/v1.15.0.tar.gz"
  SYSTEM
  EXCLUDE_FROM_ALL
  OPTIONS
  "SPDLOG_USE_STD_FORMAT=ON"
  "SPDLOG_ENABLE_PCH=ON"
)

add_library(third_party::spdlog ALIAS spdlog)