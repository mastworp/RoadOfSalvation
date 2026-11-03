cmake_minimum_required(VERSION 4.3.0)
project(
  RoadOfSalvation
  VERSION 1.0.0
  LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(ENABLE_TUI "Build the TUI layer." OFF)
option(BUILD_TESTS "Build the unit tests." ON)

include(CCache)
include(CompilerWarnings)
include(Sanitizers)
include(StagingLayout)
include(GameData)

add_subdirectory(src)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
