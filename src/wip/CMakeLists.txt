add_executable(wip WIN32 wip-main.cpp)


target_link_libraries(wip PRIVATE
  third_party::entt
  engine-main
  third_party::sokol
)


if(EMSCRIPTEN)
  target_link_options(wip PUBLIC
    -sNO_DISABLE_EXCEPTION_CATCHING
    "SHELL: --embed-file ${PROJECT_SOURCE_DIR}/config/@/config"
#    -sMODULARIZE
#    "SHELL: -s 'EXPORT_NAME=$<TARGET_PROPERTY:wip,NAME>'"
  )
endif()