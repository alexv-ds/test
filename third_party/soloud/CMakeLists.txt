#CPMAddPackage(
#  NAME SoLoud
#  VERSION ref-e82fd32
#  URL "https://github.com/jarikomppa/soloud/archive/e82fd32c1f62183922f08c14c814a02b58db1873.zip"
#  DOWNLOAD_ONLY
#)

set(SoLoud_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/soloud)

file(GLOB_RECURSE SOLOUD_SRC
  "${SoLoud_SOURCE_DIR}/src/audiosource/**.c*"
  "${SoLoud_SOURCE_DIR}/src/filter/**.c*"
  "${SoLoud_SOURCE_DIR}/src/core/**.c*"
)

add_library(soloud_static
  ${SOLOUD_SRC}
)
add_library(third_party::soloud ALIAS soloud_static)

target_include_directories(soloud_static PUBLIC "${SoLoud_SOURCE_DIR}/include")

add_compile_definitions(soloud_static PRIVATE
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:RelWithDebInfo>:DEBUG>
  $<$<CONFIG:Release>:NDEBUG>
  $<$<CONFIG:MinSizeRel>:NDEBUG>
)

target_compile_options(soloud_static PRIVATE
  -fno-rtti
  -fno-exceptions
  -Wno-reorder-ctor
  -Wno-unused-parameter
  -Wno-unused-variable
  -Wno-misleading-indentation
  -Wno-missing-braces
  -Wno-delete-non-abstract-non-virtual-dtor
  -Wno-strict-prototypes
  -Wno-unused-but-set-variable
  -Wno-char-subscripts
  -Wno-bitwise-op-parentheses
  -Wno-tautological-compare
  -Wno-newline-eof
  -Wno-unused-value
  -Wno-sign-compare
)

if (WIN32)
  target_link_libraries(soloud_static PRIVATE ole32 uuid avrt winmm)
  add_compile_definitions(soloud_static PRIVATE
    _CRT_SECURE_NO_WARNINGS
    WITH_WASAPI
    WITH_WINMM
  )
  file(GLOB_RECURSE SOLOUD_BACKENDS_SRC
    "${SoLoud_SOURCE_DIR}/src/backend/winmm/**.c*"
    "${SoLoud_SOURCE_DIR}/src/backend/wasapi/**.c*"
  )
  target_sources(soloud_static PRIVATE ${SOLOUD_BACKENDS_SRC})
elseif(EMSCRIPTEN)
  add_compile_definitions(soloud_static PRIVATE WITH_SDL2_STATIC)
  file(GLOB_RECURSE SOLOUD_BACKENDS_SRC
    "${SoLoud_SOURCE_DIR}/src/backend/sdl2_static/**.c*"
  )
  target_sources(soloud_static PRIVATE ${SOLOUD_BACKENDS_SRC})
  target_compile_options(soloud_static PUBLIC "-sUSE_SDL=2")
  target_link_options(soloud_static PUBLIC "-sUSE_SDL=2")
else()
  # TODO: add linux
  message(FATAL_ERROR "Unsupported os")
endif()

