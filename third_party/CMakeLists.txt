add_subdirectory(sokol SYSTEM EXCLUDE_FROM_ALL)
add_subdirectory(entt SYSTEM EXCLUDE_FROM_ALL)
#add_subdirectory(boost SYSTEM EXCLUDE_FROM_ALL)
add_subdirectory(spdlog SYSTEM EXCLUDE_FROM_ALL)
add_subdirectory(yaml-cpp SYSTEM EXCLUDE_FROM_ALL)


CPMAddPackage("gh:alexv-ds/loose_quadtree@2.0.0")

#add_subdirectory(soloud SYSTEM EXCLUDE_FROM_ALL)