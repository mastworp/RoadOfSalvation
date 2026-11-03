add_library(core)
add_library(RoadOfSalvation::core ALIAS core)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

target_sources(core
               PRIVATE character/player.cpp character/statistics.cpp character/profession.cpp json/json_parser.cpp
                       json/attributes_json.cpp json/professions_json.cpp
               PUBLIC FILE_SET
                      HEADERS
                      BASE_DIRS
                      inc
                      FILES
                      inc/RoadOfSalvation/character/attributes.hpp
                      inc/RoadOfSalvation/character/character.hpp
                      inc/RoadOfSalvation/character/player.hpp
                      inc/RoadOfSalvation/character/profession.hpp
                      inc/RoadOfSalvation/character/professions.hpp
                      inc/RoadOfSalvation/character/statistics.hpp
                      inc/RoadOfSalvation/json/json_parser.hpp
                      inc/RoadOfSalvation/json/attributes_json.hpp
                      inc/RoadOfSalvation/json/professions_json.hpp)
target_compile_features(core PUBLIC cxx_std_23)
target_link_libraries(core PUBLIC nlohmann_json::nlohmann_json PRIVATE RoadOfSalvation::warnings)

if(TARGET RoadOfSalvation::sanitizers)
  target_link_libraries(core PUBLIC RoadOfSalvation::sanitizers)
endif()

if(TARGET RoadOfSalvation::coverage)
  target_link_libraries(core PRIVATE RoadOfSalvation::coverage)
endif()
