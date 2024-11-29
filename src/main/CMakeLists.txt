add_executable(main main.cpp
  ../core/ServiceRegistry.cpp
  ../core/ServiceRegistry.hpp)

target_link_libraries(main PRIVATE
  third_party::entt
  third_party::sokol
  Boost::core
)
