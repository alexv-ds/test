

add_library(engine
  ServiceRegistry.hpp
  ServiceRegistry.cpp
  Engine.cpp
  Engine.hpp
  ../engine-main/main.cpp
)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_link_libraries(engine PUBLIC third_party::entt)