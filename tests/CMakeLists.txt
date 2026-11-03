include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY "https://github.com/google/googletest.git"
  GIT_TAG v1.18.0
  GIT_SHALLOW TRUE
  SYSTEM EXCLUDE_FROM_ALL FIND_PACKAGE_ARGS NAMES GTest)

FetchContent_MakeAvailable(googletest)
include(googletest)
