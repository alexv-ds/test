add_executable(wip WIN32 wip-main.cpp)


target_link_libraries(wip PRIVATE
  third_party::entt
  third_party::sokol
  engine-main
)


if(EMSCRIPTEN)
  target_link_options(wip PRIVATE
    -sNO_DISABLE_EXCEPTION_CATCHING
    -sALLOW_MEMORY_GROWTH
    "SHELL: --embed-file ${PROJECT_SOURCE_DIR}/config/@/config"
  )
endif()

#target_link_libraries(wip PRIVATE third_party::soloud)