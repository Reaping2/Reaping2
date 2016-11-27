if(CMAKE_CROSSCOMPILING)
    find_program(R2PKG_VAR r2pkg
            HINTS
                ${PROJECT_SOURCE_DIR}
                ${PROJECT_SOURCE_DIR}/build
                ${PROJECT_SOURCE_DIR}/local
                ${REAPING2_DEPS_INSTALL_DIR}
            PATH_SUFFIXES
                bin
                tools/r2pkg)
    if(NOT R2PKG_VAR)
        message(STATUS "R2PKG not found! Data package will not be generated.")
    else(NOT R2PKG_VAR)
        message(STATUS "R2PKG found! Data package will be generated.")
    endif(NOT R2PKG_VAR)

    find_program(AICOLLECTOR_VAR aicollector
            HINTS
                ${PROJECT_SOURCE_DIR}
                ${PROJECT_SOURCE_DIR}/build
                ${PROJECT_SOURCE_DIR}/local
                ${REAPING2_DEPS_INSTALL_DIR}
            PATH_SUFFIXES
                bin
                tools/aicollector)
    if (NOT AICOLLECTOR_VAR)
        message(STATUS "aicollector not found! Autoids won't be collected.")
    else (NOT AICOLLECTOR_VAR)
        message(STATUS "aicollector found! Autids will be collected.")
    endif (NOT AICOLLECTOR_VAR)
else(CMAKE_CROSSCOMPILING)
    set(R2PKG_VAR r2pkg)
    set(AICOLLECTOR_VAR aicollector)
endif(CMAKE_CROSSCOMPILING)


#set up environment for running tools in case of native windows compiling
if (WIN32 AND NOT CMAKE_CROSSCOMPILING)
    # Ensure that the tools can find their dependent DLLs
    if(MINGW)
        get_filename_component(MINGW_BINDIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    endif(MINGW)
    set(SETENV_FOR_TOOLS
        ${CMAKE_COMMAND} -E env
        "PATH=${MINGW_BINDIR}$<SEMICOLON>${REAPING2_DEPS_INSTALL_DIR}/bin$<SEMICOLON>$<TARGET_FILE_DIR:r2pkg>")
endif(WIN32 AND NOT CMAKE_CROSSCOMPILING)

#required dependencies for tool running
file(GLOB_RECURSE content "data/*")
file(GLOB_RECURSE sources "src/*")

if (AICOLLECTOR_VAR)
    #autoid collecting, update autoids.* before data.pkg
    add_custom_command(OUTPUT autoids.content autoids.src
        COMMAND ${SETENV_FOR_TOOLS}
                ${AICOLLECTOR_VAR}
                    --path ${CMAKE_CURRENT_SOURCE_DIR}/data
                    --ext json
                    --enable-json
                    --output-file ${CMAKE_CURRENT_SOURCE_DIR}/data/autoids.content
        COMMAND ${SETENV_FOR_TOOLS}
                ${AICOLLECTOR_VAR}
                    --path ${CMAKE_CURRENT_SOURCE_DIR}/../src
                    --ext cpp h
                    --enable-cpp
                    --output-file ${CMAKE_CURRENT_SOURCE_DIR}/data/autoids.src
        DEPENDS ${content} ${sources} )
endif(AICOLLECTOR_VAR)

if(R2PKG_VAR)
    #data.pkg generating
    add_custom_command(OUTPUT data.pkg
        COMMAND
            ${SETENV_FOR_UPDATE_DATA}
            ${R2PKG_VAR}
                -c ${CMAKE_CURRENT_SOURCE_DIR}/data
                -o ${PROJECT_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>/data.pkg
        DEPENDS ${content} autoids.src autoids.content )

    add_custom_target(update_data ALL
        DEPENDS autoids.content autoids.src data.pkg
        SOURCES ${content})

    set_property(TARGET update_data PROPERTY FOLDER "tools")

    install(FILES ${PROJECT_BINARY_DIR}/bin/$<LOWER_CASE:$<CONFIG>>/data.pkg DESTINATION bin)

endif(R2PKG_VAR)
