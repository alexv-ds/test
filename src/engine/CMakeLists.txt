

add_library(engine
  ServiceRegistry.hpp
  ServiceRegistry.cpp
  Engine.cpp
  Engine.hpp
  log.hpp
  service/EngineLifecycle.cpp
  service/EngineLifecycle.hpp
  SystemScheduler.cpp
  SystemScheduler.hpp
)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(engine PUBLIC third_party::entt third_party::spdlog third_party::yaml-cpp)
target_compile_definitions(engine PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)