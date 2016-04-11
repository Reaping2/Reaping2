cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Reaping2_DEPS)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/../cmake_support)

set(EXTERNALS_WORK_DIR ${PROJECT_BINARY_DIR}/externals)

if(REAPING2_DEPS_INSTALL_DIR)
    file(TO_CMAKE_PATH "${REAPING2_DEPS_INSTALL_DIR}" EXTERNALS_INSTALL_DIR)
else()
    file(TO_CMAKE_PATH "${PROJECT_SOURCE_DIR}/../local" EXTERNALS_INSTALL_DIR)
endif()
message(STATUS "Setting staging dir for ${PROJECT_NAME}: ${EXTERNALS_INSTALL_DIR}")
set(PLATFORM_CMAKE_ARGS ${PLATFORM_CMAKE_ARGS} "-DCMAKE_FIND_ROOT_PATH=${EXTERNALS_INSTALL_DIR}")

if(UNIX)
    set(PLATFORM_CMAKE_ARGS "-DCMAKE_INSTALL_RPATH=${EXERNALS_INSTALL_DIR}/lib")
endif(UNIX)

if(CMAKE_TOOLCHAIN_FILE)
    set(TOOLCHAIN_CMAKE_ARGS "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
endif()

message( STATUS "Platform CMAKE arguments: ${PLATFORM_CMAKE_ARGS}" )
message( STATUS "Toolchain CMAKE arguments: ${TOOLCHAIN_CMAKE_ARGS}" )

# Most often we want to build everything on Windows, but not on Linux. It can
# be still overridden by specifying the -DUSE_BUNDLED_DEFAULT option with the
# cmake command.
if(WIN32)
    set(DEFAULT_ENABLED_BUNDLED_DEPS ON)
else()
    set(DEFAULT_ENABLED_BUNDLED_DEPS OFF)
endif()

option(USE_BUNDLED_DEFAULT
        "Set default option value for bundled dependencies"
        ${DEFAULT_ENABLED_BUNDLED_DEPS})

option(USE_BUNDLED_LIBWHEREAMI
    "Use bundled libwhereami"
    ON)

# Shorthand for declaring an option and adding a subdirectory if option is on
macro(ADD_BUNDLED_DEPENDENCY NAME)
    string(TOUPPER ${NAME} NAME_UPPER)

    option(USE_BUNDLED_${NAME_UPPER}
        "Use bundled ${name}"
        ${USE_BUNDLED_DEFAULT})

    if(USE_BUNDLED_${NAME_UPPER})
        message(STATUS "Adding external: ${NAME}")
        add_subdirectory(${NAME})
    endif()
endmacro(ADD_BUNDLED_DEPENDENCY)

include(ExternalProject)

add_bundled_dependency(boost)
add_bundled_dependency(glew)
add_bundled_dependency(glfw)
add_bundled_dependency(glm)
add_bundled_dependency(libogg)
add_bundled_dependency(libvorbis)
add_bundled_dependency(zlib)
add_bundled_dependency(libpng)
add_bundled_dependency(portaudio)
add_bundled_dependency(enet)
add_bundled_dependency(libssh2)
add_bundled_dependency(libcurl)
add_bundled_dependency(libwhereami)
if(WIN32)
    add_bundled_dependency(dbg)
else(WIN32)
    add_bundled_dependency(libunwind)
endif(WIN32)

