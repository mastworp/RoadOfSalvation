include_guard(GLOBAL)

option(ENABLE_STATIC_LINKING "Link the game statically so it runs on any Linux distribution." OFF)

if(NOT ENABLE_STATIC_LINKING)
  return()
endif()

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(FATAL_ERROR "ENABLE_STATIC_LINKING is only supported on Linux.")
endif()

if(ENABLE_SANITIZERS)
  message(FATAL_ERROR "ENABLE_STATIC_LINKING cannot be combined with ENABLE_SANITIZERS.")
endif()

add_library(static_linking INTERFACE)
add_library(RoadOfSalvation::static_linking ALIAS static_linking)
target_link_options(static_linking INTERFACE -static)
message(STATUS "Static linking is enabled.")
