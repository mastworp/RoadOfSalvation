include_guard(GLOBAL)

option(ENABLE_COVERAGE "Instrument the build for gcov code coverage." OFF)

if(NOT ENABLE_COVERAGE)
  return()
endif()

set(COVERAGE_MIN_LINES 95 CACHE STRING "Minimum line coverage percentage enforced by the coverage target.")
set(COVERAGE_MIN_FUNCTIONS 95 CACHE STRING "Minimum function coverage percentage enforced by the coverage target.")
set(COVERAGE_MIN_BRANCHES 50 CACHE STRING "Minimum branch coverage percentage enforced by the coverage target.")

if(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  message(FATAL_ERROR "Code coverage requires gcc or clang.")
endif()

find_program(GCOVR_EXECUTABLE gcovr)
if(NOT GCOVR_EXECUTABLE)
  message(FATAL_ERROR "Code coverage requires gcovr: pip install -r requirements-dev.txt")
endif()

string(REGEX MATCH "^[0-9]+" coverage_compiler_major "${CMAKE_CXX_COMPILER_VERSION}")
cmake_path(GET CMAKE_CXX_COMPILER PARENT_PATH coverage_compiler_dir)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  find_program(LLVM_COV_EXECUTABLE NAMES llvm-cov-${coverage_compiler_major} llvm-cov HINTS "${coverage_compiler_dir}")
  set(coverage_gcov_command "${LLVM_COV_EXECUTABLE} gcov")
else()
  find_program(GCOV_EXECUTABLE NAMES gcov-${coverage_compiler_major} gcov HINTS "${coverage_compiler_dir}")
  set(coverage_gcov_command "${GCOV_EXECUTABLE}")
endif()

if(coverage_gcov_command MATCHES "NOTFOUND")
  message(FATAL_ERROR "Code coverage requires gcov for gcc or llvm-cov for clang.")
endif()

add_library(coverage_flags INTERFACE)
add_library(RoadOfSalvation::coverage ALIAS coverage_flags)
target_compile_options(coverage_flags INTERFACE --coverage)
target_link_options(coverage_flags INTERFACE --coverage)

set(COVERAGE_OUTPUT_DIR "${PROJECT_BINARY_DIR}/coverage")

add_custom_target(coverage
                  COMMAND ${CMAKE_COMMAND} -E make_directory "${COVERAGE_OUTPUT_DIR}"
                  COMMAND "${GCOVR_EXECUTABLE}" --root "${PROJECT_SOURCE_DIR}" --object-directory
                          "${PROJECT_BINARY_DIR}" --filter src/ --gcov-executable "${coverage_gcov_command}"
                          --exclude-unreachable-branches --exclude-throw-branches --txt --print-summary --html-details
                          "${COVERAGE_OUTPUT_DIR}/index.html" --cobertura "${COVERAGE_OUTPUT_DIR}/cobertura.xml"
                          --json-summary "${COVERAGE_OUTPUT_DIR}/summary.json" --markdown-summary
                          "${COVERAGE_OUTPUT_DIR}/summary.md" --fail-under-line ${COVERAGE_MIN_LINES}
                          --fail-under-function ${COVERAGE_MIN_FUNCTIONS} --fail-under-branch ${COVERAGE_MIN_BRANCHES}
                  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
                  COMMENT "Collecting code coverage into ${COVERAGE_OUTPUT_DIR}"
                  VERBATIM)

message(STATUS "Code coverage is enabled.")
