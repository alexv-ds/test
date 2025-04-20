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

CPMAddPackage(
  NAME ImGui
  URL "https://github.com/ocornut/imgui/archive/refs/tags/v1.91.3.tar.gz"
  DOWNLOAD_ONLY
)

add_library(sokol-libs STATIC EXCLUDE_FROM_ALL
  sokol.cpp
  ${ImGui_SOURCE_DIR}/imgui.cpp
  ${ImGui_SOURCE_DIR}/imgui_demo.cpp
  ${ImGui_SOURCE_DIR}/imgui_draw.cpp
  ${ImGui_SOURCE_DIR}/imgui_tables.cpp
  ${ImGui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(sokol-libs SYSTEM PUBLIC ${ImGui_SOURCE_DIR})
target_include_directories(sokol-libs SYSTEM PUBLIC ${sokol_SOURCE_DIR} ${sokol-gp_SOURCE_DIR})
target_compile_definitions(sokol-libs PRIVATE
  $<$<CONFIG:Debug>:SOKOL_DEBUG>
  $<$<CONFIG:RelWithDebInfo>:SOKOL_DEBUG>
  $<$<CONFIG:Release>:NDEBUG>
  $<$<CONFIG:MinSizeRel>:NDEBUG>
)

if(WIN32)
  target_compile_definitions(sokol-libs PRIVATE SOKOL_D3D11)
elseif(LINUX)
  target_compile_definitions(sokol-libs PRIVATE SOKOL_GLCORE)
  target_link_libraries(sokol-libs PRIVATE X11 Xi Xcursor GL m dl)
elseif(EMSCRIPTEN)
  target_compile_definitions(sokol-libs PRIVATE SOKOL_GLES3)
  target_link_options(sokol-libs PUBLIC "-sFULL_ES3")
else()
  message(FATAL_ERROR "Unsupported os")
endif()

add_library(third_party::sokol ALIAS sokol-libs)

