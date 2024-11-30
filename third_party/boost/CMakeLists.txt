CPMAddPackage(
  NAME Boost
  VERSION 1.87.0.beta1
  URL "https://github.com/boostorg/boost/releases/download/boost-1.87.0.beta1/boost-1.87.0.beta1-cmake.7z"
  SYSTEM
  EXCLUDE_FROM_ALL
  OPTIONS
  "BOOST_STACKTRACE_ENABLE_WINDBG=ON"
)
