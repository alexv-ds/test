

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
  systems/UpdateMap.cpp
  systems/UpdateMap.hpp
  components/other.hpp
  systems/RenderSokolBegin.cpp
  systems/RenderSokolBegin.hpp
  systems/RenderSokolEnd.cpp
  systems/RenderSokolEnd.hpp
  systems/RenderSokolDraw.cpp
  systems/RenderSokolDraw.hpp
  systems/InputWASDPositionController.cpp
  systems/InputWASDPositionController.hpp
  systems/SyncCameraWithMainWindow.cpp
  systems/SyncCameraWithMainWindow.hpp
  systems/InputQEScaleController.cpp
  systems/InputQEScaleController.hpp
  systems/simple_systems.cpp
  systems/simple_systems.hpp
  helpers/TimeMeter.hpp
  ModuleDefine.hpp
  modules/sound/components.hpp
  modules/sound.hpp
  modules/sound/sound-define.cpp
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
)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_link_libraries(engine PUBLIC third_party::entt third_party::spdlog third_party::yaml-cpp)
target_link_libraries(engine PRIVATE loose_quadtree::loose_quadtree third_party::sokol)
target_compile_definitions(engine PUBLIC SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)