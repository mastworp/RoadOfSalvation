include(FetchContent)

set(INSTALL_GTEST OFF)

FetchContent_Declare(googletest
                     GIT_REPOSITORY "https://github.com/google/googletest.git"
                     GIT_TAG v1.18.0
                     GIT_SHALLOW TRUE
                     SYSTEM EXCLUDE_FROM_ALL FIND_PACKAGE_ARGS NAMES GTest)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(core_tests)
target_sources(core_tests
               PRIVATE attributes_json_test.cpp
                       game_data_test.cpp
                       json_parser_test.cpp
                       player_test.cpp
                       profession_test.cpp
                       professions_json_test.cpp
                       statistics_test.cpp
                       test_support.hpp)
target_link_libraries(core_tests PRIVATE RoadOfSalvation::core RoadOfSalvation::warnings GTest::gmock_main)
gtest_discover_tests(core_tests DISCOVERY_MODE PRE_TEST NO_PRETTY_VALUES)

add_test(NAME RoadOfSalvation.smoke COMMAND RoadOfSalvation)
set_tests_properties(RoadOfSalvation.smoke PROPERTIES PASS_REGULAR_EXPRESSION "The heroes name is: hero")
