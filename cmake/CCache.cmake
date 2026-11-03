option(USE_CCACHE "Use ccache as a compiler launcher." ON)

if(NOT USE_CCACHE)
  return()
endif()

find_program(CCACHE_PROGRAM ccache)
if(NOT CCACHE_PROGRAM)
  message(STATUS "CCache wasn't found. Continuing without.")
  return()
endif()

set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
message(STATUS "ccache enabled set USE_CCACHE=off to disable it.")
