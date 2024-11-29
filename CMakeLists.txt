cmake_minimum_required(VERSION 3.30)

project(Test
  VERSION 0.0.1
  LANGUAGES CXX C
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic)

include(cmake/CPM.cmake)

add_subdirectory(src/sokol SYSTEM EXCLUDE_FROM_ALL)
add_subdirectory(src/entt SYSTEM EXCLUDE_FROM_ALL)
add_subdirectory(src/boost SYSTEM EXCLUDE_FROM_ALL)
add_subdirectory(src/main)
add_subdirectory(src/wip)
