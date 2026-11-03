option(ENABLE_SANITIZERS "Build with ASan + UBSan" ON)

if(NOT ENABLE_SANITIZERS)
  return()
endif()

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  message(FATAL_ERROR "Sanitizers require gcc or clang.")
  return()
endif()

add_library(sanitizers INTERFACE)
add_library(RoadOfSalvation::sanitizers ALIAS sanitizers)
target_compile_options(
  sanitizers INTERFACE -fsanitize=address,undefined -fno-omit-frame-pointer
                       -fno-sanitize-recover=all)
target_link_options(sanitizers INTERFACE -fsanitize=address,undefined)
message(STATUS "Sanitizers are enabled.")
