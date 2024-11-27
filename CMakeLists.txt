cmake_minimum_required(VERSION 3.30)

project(Test
  VERSION 0.0.1
  LANGUAGES CXX C
)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra -pedantic)

add_executable(main main/main.cpp)

include(cmake/CPM.cmake)

CPMAddPackage(
  NAME cmake-git-version-tracking
  VERSION ref-3605a6
  URL "https://github.com/andrew-hardin/cmake-git-version-tracking/archive/6c0cb87edd029ddfb403a8e24577c144a03605a6.tar.gz"
  URL_HASH "SHA3_256=c38652cf6e5799fabb976d05973040374dddc75de2fa1f1f34ac2286fcf209d0"
  SYSTEM YES
  EXCLUDE_FROM_ALL YES
)

target_link_libraries(main PRIVATE cmake_git_version_tracking)