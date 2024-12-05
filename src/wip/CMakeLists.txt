add_executable(wip WIN32 wip-main.cpp)


target_link_libraries(wip PRIVATE
  third_party::entt
  engine-main
  third_party::sokol
)
