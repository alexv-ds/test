CPMAddPackage(
  NAME EnTT
  URL "https://github.com/skypjack/entt/archive/refs/tags/v3.14.0.tar.gz"
)

add_library(third_party::entt ALIAS EnTT)

#add_library(sokol-libs STATIC EXCLUDE_FROM_ALL
#  sokol.c
#)
#target_include_directories(sokol-libs SYSTEM PUBLIC ${sokol_SOURCE_DIR} ${sokol-gp_SOURCE_DIR})
#target_link_libraries(sokol-libs PRIVATE X11 Xi Xcursor GL m dl)
#
#add_library(third_party::sokol ALIAS sokol-libs)
#
