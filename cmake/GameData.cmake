include_guard(GLOBAL)
include(GNUInstallDirs)

function(add_game_data target)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "SOURCE_DIR" "FILES")
    if(NOT arg_SOURCE_DIR OR NOT arg_FILES)
        message(FATAL_ERROR "SOURCE_DIR and FILES are required")
    endif()

    cmake_path(ABSOLUTE_PATH arg_SOURCE_DIR
            BASE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
            NORMALIZE OUTPUT_VARIABLE srcDir)
    set(dstDir ${ROADOFSALVATION_STAGE_DIR}/${ROADOFSALVATION_DATA_INSTALL_DIR})

    set(staged "")
    foreach(f IN LISTS arg_FILES)
        cmake_path(GET f PARENT_PATH subDir)
        add_custom_command(
                OUTPUT  ${dstDir}/${f}
                COMMAND ${CMAKE_COMMAND} -E make_directory ${dstDir}/${subDir}
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${srcDir}/${f} ${dstDir}/${f}
                DEPENDS ${srcDir}/${f}
                COMMENT "Staging game data: ${f}"
                VERBATIM)
        list(APPEND staged ${dstDir}/${f})
        install(FILES ${srcDir}/${f} DESTINATION ${ROADOFSALVATION_DATA_INSTALL_DIR}/${subDir})
    endforeach()

    add_custom_target(${target}_data DEPENDS ${staged})
    add_dependencies(${target} ${target}_data)

    cmake_path(RELATIVE_PATH ROADOFSALVATION_DATA_INSTALL_DIR
            BASE_DIRECTORY ${CMAKE_INSTALL_BINDIR}
            OUTPUT_VARIABLE relPath)
    target_compile_definitions(${target} PRIVATE ROADOFSALVATION_DATA_REL_PATH="${relPath}")
endfunction()
