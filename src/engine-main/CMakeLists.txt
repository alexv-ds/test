add_library(engine-main STATIC
  engine-main.cpp
)

target_link_libraries(engine-main PUBLIC engine)

target_link_libraries(engine-main PRIVATE third_party::sokol)