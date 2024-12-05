add_executable(main WIN32
  main.cpp
)

target_link_libraries(main PRIVATE
  third_party::entt
  third_party::sokol
  engine
)
