add_library(engine-main STATIC
  engine-main.cpp
  SokolStatusImpl.hpp
  InputImpl.cpp
  InputImpl.hpp
)

target_link_libraries(engine-main PUBLIC engine)

target_link_libraries(engine-main PRIVATE third_party::sokol)