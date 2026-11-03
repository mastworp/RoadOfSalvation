add_subdirectory(app)

if(ENABLE_TUI)
  add_subdirectory(tui)
endif()

add_executable(RoadOfSalvation main.cpp)
set_property(TARGET RoadOfSalvation PROPERTY DEBUGGER_WORKING_DIRECTORY ${ROADOFSALVATION_STAGE_DIR})
target_link_libraries(RoadOfSalvation PRIVATE RoadOfSalvation::core RoadOfSalvation::warnings)

add_game_data(core SOURCE_DIR data FILES professions.json)
install(TARGETS RoadOfSalvation)
