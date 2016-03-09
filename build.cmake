cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# ------------------------------------------------------------------------------

# Macro: PRINT_HELP
#
#        Print script usage and options.
#
macro(PRINT_HELP)
    string(CONFIGURE ${BUILD_SCRIPT_OPTIONS_DOC} OPTIONS_DOC @ONLY )
    message(
"Usage: cmake [CMAKE VARS].. -P ${BASENAME} [OPTION=VAL]...

Platform-independent build script for Reaping2.

Main features:
 - any cmake variable set for the script mode (i.e. '-DVAR=val') is passed
   on to the cmake commands when configuring each project
 - the script has its own option arguments (see below); these are in the
   form: 'strname=value', without any preceeding '-' or '--' (as those might
   be interpreted by cmake itself).
 - for any option, if only the name is present without setting its value
   with '=VALUE'; it will be automatically set to 'ON'; a prime example
   is: cmake -P build.cmake help

OPTIONS

${OPTIONS_DOC}

EXAMPLES

  cmake -P cmake.build

    Build the project with the default settings (see above for the per-option
    defaults). The dependencies will be only built if the folder they would be
    installed doesn't exist. The default build configuration is: ${DEFAULT_BUILD_CONFIG}.

  cmake -DCMAKE_BUILD_TYPE=Debug -P cmake.build depsdir=build/debs-debug r2dir=build/debug

    This demonstrates changing the build type (note that it is set as a CMake
    option, with -D), as well as changing the default build folders.

  cmake -DREAPING2_DEPS_INSTALL_DIR=$PWD/local_release -P cmake.build deps r2=off

    Install dependencies to a non-default location, and force to build the deps
    project (regardless of the existence of the install dir); also, do not build
    the main Reaping2 project.
")
endmacro(PRINT_HELP)

# Macro: PROCESS_ARGS
#
#        Stores arguments on the left side of '-P <SCRIPT>', which will be
#        re-used for the CMake configuration steps during build.
#        Remaining arguments are processed as options for this build script.
#
macro(PROCESS_ARGS ConfigArgsOut)

    foreach(ARG_INDEX RANGE 1 ${CMAKE_ARGC})
        list(APPEND ARGLIST ${CMAKE_ARGV${ARG_INDEX}})
    endforeach()

    string(FIND "${ARGLIST}" "-P;${BASENAME}" SEP_POS)

    string(SUBSTRING "${ARGLIST}" 0 ${SEP_POS} ${ConfigArgsOut})

    if(NOT ${SEP_POS} EQUAL -1)
        math(EXPR SEP_END_POS "${SEP_POS} + 14")
        string(SUBSTRING "${ARGLIST}" ${SEP_END_POS} "-1" BUILD_SCRIPT_ARGS)

        foreach(SARG ${BUILD_SCRIPT_ARGS})
            string(FIND "${SARG}" "=" SEP_POS)
            string(SUBSTRING "${SARG}" 0 ${SEP_POS} CURRENT_SCRIPT_ARG_NAME)
            string(TOUPPER ${CURRENT_SCRIPT_ARG_NAME} CURRENT_SCRIPT_ARG_NAME)
            if(NOT ${SEP_POS} EQUAL -1)
                math(EXPR SEP_END_POS "${SEP_POS} + 1")
                string(SUBSTRING "${SARG}" ${SEP_END_POS} -1 CURRENT_SCRIPT_ARG_VALUE)
                if(DEFINED BUILD_SCRIPT_OPTION_${CURRENT_SCRIPT_ARG_NAME})
                    set(BUILD_SCRIPT_OPTION_${CURRENT_SCRIPT_ARG_NAME} ${CURRENT_SCRIPT_ARG_VALUE})
                endif()
            else(NOT ${SEP_POS} EQUAL -1)
                if(DEFINED BUILD_SCRIPT_OPTION_${CURRENT_SCRIPT_ARG_NAME})
                    set(BUILD_SCRIPT_OPTION_${CURRENT_SCRIPT_ARG_NAME} ON)
                endif()
            endif(NOT ${SEP_POS} EQUAL -1)
        endforeach()
    endif()

endmacro(PROCESS_ARGS)

# Macro: BUILD_PROJECT
#
#        Run a CMake configure step, then a build step with the given source & binary dirs.
#
macro(BUILD_PROJECT HomeDir BuildDir)

    execute_process(
        COMMAND ${CMAKE_COMMAND}
            -H${HomeDir}
            -B${BuildDir}
            ${CONFIG_ARGS}
        WORKING_DIRECTORY ${ROOT_DIR})

    execute_process(
        COMMAND ${CMAKE_COMMAND}
            --build ${BuildDir}
            --config ${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY ${ROOT_DIR})

endmacro(BUILD_PROJECT)

# Macro: ADD_SCRIPT_OPTION
#
#        Set a CMake variable named with the 'BUILD_SCRIPT_OPTION_' prefix, and the
#        upper-case name of the option; with the value given as the argument.
#        Additionally, record the option for the help output.
#
macro(ADD_SCRIPT_OPTION OptionName DefaultValue HelpDoc)

    string(TOUPPER ${OptionName} OptionNameUpper)
    set(BUILD_SCRIPT_OPTION_${OptionNameUpper} ${DefaultValue})

    # Pretty-format help doc

    set(SPACING "           ")
    string(LENGTH "${SPACING}" SPACING_MAX)
    string(LENGTH "${OptionName}" OPT_LEN)
    math(EXPR SPACING_LEN "${SPACING_MAX} - ${OPT_LEN}")
    string(SUBSTRING "${SPACING}" 0 ${SPACING_LEN} SPACING_1ST)
    set(BUILD_SCRIPT_OPTIONS_DOC "${BUILD_SCRIPT_OPTIONS_DOC}  ${OptionName}${SPACING_1ST}${HelpDoc}  (@BUILD_SCRIPT_OPTION_${OptionNameUpper}@)")
    foreach(DOCSTR ${ARGN})
        set(BUILD_SCRIPT_OPTIONS_DOC "${BUILD_SCRIPT_OPTIONS_DOC}\n    ${SPACING}${DOCSTR}")
    endforeach(DOCSTR)
    set(BUILD_SCRIPT_OPTIONS_DOC "${BUILD_SCRIPT_OPTIONS_DOC}\n")

endmacro(ADD_SCRIPT_OPTION OptionName DefaultValue)

# ------------------------------------------------------------------------------

get_filename_component(BASENAME ${CMAKE_SCRIPT_MODE_FILE} NAME)
get_filename_component(ROOT_DIR ${CMAKE_SCRIPT_MODE_FILE} DIRECTORY)
set(DEFAULT_BUILD_CONFIG Release)

add_script_option(help OFF "print help and exit")
add_script_option(deps MAYBE "build the Reaping2_DEPS project" 
                             "maybe means: if the deps install dir exists, the build won't be triggered"
                             "if 'deps=on' is explicitly given, the build is triggered regardless")
add_script_option(depsdir ${ROOT_DIR}/build-deps "default build directory for Reaping2_DEPS")
add_script_option(r2 ON "build the Reaping2 project.")
add_script_option(r2dir ${ROOT_DIR}/build "default build directory for Reaping2")

# ------------------------------------------------------------------------------

# Optional local config; for example it might contain lines like:
#   set(REAPING2_DEPS_INSTALL_DIR ${ROOT_DIR}/local-vc140-x86-release)
#   set(BUILD_SCRIPT_OPTION_R2DIR ${ROOT_DIR}/build/vc140-x86/release)
#   set(BUILD_SCRIPT_OPTION_DEPSDIR ${ROOT_DIR}/build/deps/vc140-x86/release)
if(EXISTS .buildrc.cmake)
    include(.buildrc.cmake)
endif(EXISTS .buildrc.cmake)

process_args(CONFIG_ARGS)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    set(CONFIG_ARGS "-DCMAKE_BUILD_TYPE=${DEFAULT_BUILD_CONFIG}" ${CONFIG_ARGS})
endif(NOT DEFINED CMAKE_BUILD_TYPE)

if(NOT DEFINED REAPING2_DEPS_INSTALL_DIR AND EXISTS ${ROOT_DIR}/local)
    set(REAPING2_DEPS_INSTALL_DIR ${ROOT_DIR}/local)
endif()
list(APPEND CONFIG_ARGS "-DREAPING2_DEPS_INSTALL_DIR=${REAPING2_DEPS_INSTALL_DIR}")

# ------------------------------------------------------------------------------

# Print help (and stop processing)

if(${BUILD_SCRIPT_OPTION_HELP})
    print_help()
    return()
endif(${BUILD_SCRIPT_OPTION_HELP})

# Build Reaping2_DEPS if needed

if("${BUILD_SCRIPT_OPTION_DEPS}" STREQUAL "MAYBE")
    if(EXISTS ${REAPING2_DEPS_INSTALL_DIR})
        set(BUILD_SCRIPT_OPTION_DEPS OFF)
    else(EXISTS ${REAPING2_DEPS_INSTALL_DIR})
        set(BUILD_SCRIPT_OPTION_DEPS ON)
    endif(EXISTS ${REAPING2_DEPS_INSTALL_DIR})
endif("${BUILD_SCRIPT_OPTION_DEPS}" STREQUAL "MAYBE")

if(${BUILD_SCRIPT_OPTION_DEPS})
    message(STATUS "--------------------------------------------------")
    message(STATUS "Building Reaping2_DEPS ..")
    message(STATUS "\tsource dir: ${ROOT_DIR}/deps")
    message(STATUS "\tbuild dir: ${BUILD_SCRIPT_OPTION_DEPSDIR}")
    message(STATUS "\tdeps install dir: ${REAPING2_DEPS_INSTALL_DIR}")
    message(STATUS "\tcmake opts: ${CONFIG_ARGS}")
    message(STATUS "--------------------------------------------------")
    build_project(${ROOT_DIR}/deps ${BUILD_SCRIPT_OPTION_DEPSDIR})
endif(${BUILD_SCRIPT_OPTION_DEPS})

# Build Reaping2 if needed

if(${BUILD_SCRIPT_OPTION_R2})
    message(STATUS "--------------------------------------------------")
    message(STATUS "Building Reaping2 ..")
    message(STATUS "\tsource dir: ${ROOT_DIR}")
    message(STATUS "\tbuild dir: ${BUILD_SCRIPT_OPTION_R2DIR}")
    message(STATUS "\tdeps install dir: ${REAPING2_DEPS_INSTALL_DIR}")
    message(STATUS "\tcmake opts: ${CONFIG_ARGS}")
    message(STATUS "--------------------------------------------------")
    build_project(${ROOT_DIR} ${BUILD_SCRIPT_OPTION_R2DIR})
endif(${BUILD_SCRIPT_OPTION_R2})

