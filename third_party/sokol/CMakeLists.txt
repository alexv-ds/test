CPMAddPackage(
  NAME sokol
  VERSION ref-885e15
  URL "https://github.com/floooh/sokol/archive/6f8121e6aa523b0c0a3e9d7212073bab9a885e15.tar.gz"
  DOWNLOAD_ONLY
)

CPMAddPackage(
  NAME sokol-gp
  URL "https://github.com/edubart/sokol_gp/archive/refs/tags/v0.6.0.tar.gz"
  DOWNLOAD_ONLY
)

add_library(sokol-libs STATIC EXCLUDE_FROM_ALL
  sokol.c
)
target_include_directories(sokol-libs SYSTEM PUBLIC ${sokol_SOURCE_DIR} ${sokol-gp_SOURCE_DIR})

if(WIN32)

elseif(LINUX)
  target_link_libraries(sokol-libs PRIVATE X11 Xi Xcursor GL m dl)
else()
  message(FATAL_ERROR "Unsupported os")
endif()

add_library(third_party::sokol ALIAS sokol-libs)

