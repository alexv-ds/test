CPMAddPackage(
  NAME yaml-cpp
  VERSION ref-5d6608
  URL https://github.com/jbeder/yaml-cpp/archive/9ce5a25188d83b43dd5cd633f2975be10f5d6608.tar.gz
  SYSTEM
  EXCLUDE_FROM_ALL
)

add_library(third_party::yaml-cpp ALIAS yaml-cpp)