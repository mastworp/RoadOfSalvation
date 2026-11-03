include_guard(GLOBAL)

find_program(CLANG_FORMAT_EXECUTABLE clang-format)
find_program(CMAKE_FORMAT_EXECUTABLE cmake-format)

file(GLOB_RECURSE
     format_cxx_sources
     CONFIGURE_DEPENDS
     "${PROJECT_SOURCE_DIR}/src/*.cpp"
     "${PROJECT_SOURCE_DIR}/src/*.hpp"
     "${PROJECT_SOURCE_DIR}/tests/*.cpp"
     "${PROJECT_SOURCE_DIR}/tests/*.hpp")

file(GLOB_RECURSE format_cmake_sources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/cmake/*.cmake"
     "${PROJECT_SOURCE_DIR}/src/CMakeLists.txt" "${PROJECT_SOURCE_DIR}/tests/CMakeLists.txt")
list(PREPEND format_cmake_sources "${PROJECT_SOURCE_DIR}/CMakeLists.txt")

add_custom_target(format)
add_custom_target(format-check)

function(add_format_targets name executable)
  cmake_parse_arguments(PARSE_ARGV 2 arg "" "" "FIX;CHECK;FILES")

  if(executable)
    add_custom_target(${name}
                      COMMAND "${executable}" ${arg_FIX} ${arg_FILES}
                      WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                      VERBATIM)
    add_custom_target(${name}-check
                      COMMAND "${executable}" ${arg_CHECK} ${arg_FILES}
                      WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                      VERBATIM)
  else()
    message(STATUS "${name} wasn't found. The ${name} targets will fail.")
    foreach(target IN ITEMS ${name} ${name}-check)
      add_custom_target(${target}
                        COMMAND ${CMAKE_COMMAND} -E echo "${name} wasn't found: pip install -r requirements-dev.txt"
                        COMMAND ${CMAKE_COMMAND} -E false
                        VERBATIM)
    endforeach()
  endif()

  add_dependencies(format ${name})
  add_dependencies(format-check ${name}-check)
endfunction()

add_format_targets(clang-format
                   "${CLANG_FORMAT_EXECUTABLE}"
                   FIX
                   -i
                   CHECK
                   --dry-run
                   --Werror
                   FILES
                   ${format_cxx_sources})

add_format_targets(cmake-format
                   "${CMAKE_FORMAT_EXECUTABLE}"
                   FIX
                   -i
                   CHECK
                   --check
                   FILES
                   ${format_cmake_sources})
