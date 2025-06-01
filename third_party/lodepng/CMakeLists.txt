add_library(lodepng
  lodepng/lodepng.cpp
  lodepng/lodepng.h
  lodepng/lodepng_util.cpp
  lodepng/lodepng_util.h
)
add_library(third_party::lodepng ALIAS lodepng)

target_include_directories(lodepng PUBLIC lodepng)
