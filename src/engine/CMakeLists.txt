

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
  modules/graphics/components.hpp
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
  systems/UpdateMap.cpp
  systems/UpdateMap.hpp
  components/other.hpp
  modules/graphics/systems/RenderSokolBegin.cpp
  modules/graphics/systems/RenderSokolBegin.hpp
  modules/graphics/systems/RenderSokolEnd.cpp
  modules/graphics/systems/RenderSokolEnd.hpp
  modules/graphics/systems/RenderSokolDraw.cpp
  modules/graphics/systems/RenderSokolDraw.hpp
  systems/InputWASDPositionController.cpp
  systems/InputWASDPositionController.hpp
  modules/graphics/systems/SyncCameraWithMainWindow.cpp
  modules/graphics/systems/SyncCameraWithMainWindow.hpp
  systems/InputQEScaleController.cpp
  systems/InputQEScaleController.hpp
  systems/simple_systems.cpp
  systems/simple_systems.hpp
  helpers/TimeMeter.hpp
  ModuleDefine.hpp
  ModuleRegistry.hpp
  ModuleLoader.hpp
  ModuleRegistryImpl.cpp
  ModuleRegistryImpl.hpp
  modules/resource/resource-define.cpp
  modules/resource.hpp
  helpers/sokol_log.cpp
  helpers/sokol_log.hpp
  modules/resource/services/Loader.cpp
  modules/resource/services/Loader.hpp
  modules/graphics.hpp
  modules/graphics/graphics-define.cpp
  modules/graphics/components_private.hpp
  modules/graphics/systems/TextureLoader.cpp
  modules/graphics/systems/TextureLoader.hpp
)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(engine PUBLIC third_party::entt third_party::spdlog third_party::yaml-cpp)
target_link_libraries(engine PRIVATE
  loose_quadtree::loose_quadtree
  third_party::sokol
  third_party::lodepng
)
target_compile_definitions(engine PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)