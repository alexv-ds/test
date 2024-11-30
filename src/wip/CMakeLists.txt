add_executable(wip WIN32 main.cpp)


target_link_libraries(wip PRIVATE
  third_party::entt
  engine
  third_party::sokol
)
