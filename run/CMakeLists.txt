file(GLOB_RECURSE data_src "data/*")

if(WIN32)
    # Ensure that r2pkg can find its dependent DLLs
    set(SETENV_FOR_UPDATE_DATA
            ${CMAKE_COMMAND} -E env
            "PATH=${REAPING2_DEPS_INSTALL_DIR}/bin$<SEMICOLON>$<TARGET_FILE_DIR:r2pkg>")
endif(WIN32)

add_custom_command(OUTPUT data.pkg
        COMMAND
            ${SETENV_FOR_UPDATE_DATA}
            r2pkg -c ${CMAKE_CURRENT_SOURCE_DIR}/data
        DEPENDS ${data_src})

add_custom_target(update_data ALL
        DEPENDS data.pkg
        SOURCES ${data_src})
