cmake_minimum_required(VERSION 3.30)

project(Test
  VERSION 0.0.1
  LANGUAGES CXX C
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-fno-rtti -Wall -Wextra -pedantic -Wno-language-extension-token)

include(cmake/CPM.cmake)

add_subdirectory(third_party)
add_subdirectory(src/engine)
add_subdirectory(src/engine-main)
add_subdirectory(src/main)
add_subdirectory(src/wip)
