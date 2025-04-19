

add_library(engine
  ServiceRegistry.hpp
  ServiceRegistry.cpp
  Engine.cpp
  Engine.hpp
  log.hpp
  EngineLifecycle.cpp
  EngineLifecycle.hpp
  SystemScheduler.cpp
  SystemScheduler.hpp
  System.hpp
  System.cpp
  EventEmitter.cpp
  EventEmitter.hpp
  components/world.hpp
  components/graphics.hpp
  Map.cpp
  Map.hpp
  noncopyable.hpp
  MapImpl.cpp
  MapImpl.hpp
  systems/UpdateBBox.hpp
  systems/UpdateBBox.cpp
  SokolStatus.hpp
  hsv_rgb_conversion.hpp
  hsv_rgb_conversion.cpp
  Input.cpp
  Input.hpp
)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(engine PUBLIC third_party::entt third_party::spdlog third_party::yaml-cpp)
target_link_libraries(engine PRIVATE loose_quadtree::loose_quadtree)
target_compile_definitions(engine PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)