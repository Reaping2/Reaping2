cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_VERBOSE_MAKEFILE ON)

set( CMAKE_CXX_STANDARD 11 )

# support for YouCompleteMe
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set( CMAKE_CXX_FLAGS "-Wno-deprecated-declarations" )
if($ENV{REAPING2_PROFILE})
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg" )
endif($ENV{REAPING2_PROFILE})

project(Reaping2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake_support)

if(NOT DEFINED REAPING2_DEPS_INSTALL_DIR AND EXISTS ${PROJECT_SOURCE_DIR}/local)
    set(REAPING2_DEPS_INSTALL_DIR ${PROJECT_SOURCE_DIR}/local)
endif()

set(CMAKE_FIND_ROOT_PATH ${CMAKE_FIND_ROOT_PATH} ${REAPING2_DEPS_INSTALL_DIR})

include( FindDeps ) 
include( CMakeHelper )

set(BUILD_SHARED_LIBS ON)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)
if(WIN32)
    set( WIN32_ENET_INCLUDE_WS2_32 ws2_32.lib )
    set( WIN32_ENET_INCLUDE_WINMM winmm.lib )
    if(MINGW)
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj" )
    else(MINGW)
        # Had to use this when compiled debug+64bit with msvc
        # see: https://msdn.microsoft.com/en-us/library/8578y171.aspx
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj" )
    endif(MINGW)
endif(WIN32)

link_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/bin/libs"
    ${Boost_LIBRARY_DIRS}
    )

# use ccache if available
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

include_directories(
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/deps/json-cpp-0.6.0-rc2"
    "${PROJECT_SOURCE_DIR}/deps/eos_portable_archive/eos"
    ${Boost_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${LIBOGG_INCLUDE_DIR}
    ${LIBVORBIS_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
    ${PNG_INCLUDE_DIR}
    ${PORTAUDIO_INCLUDE_DIR}
    ${PORTAUDIO_CMAKE_INCLUDE_DIR}
    ${WIN32_ENET_INCLUDE_WS2_32}
    ${WIN32_ENET_INCLUDE_WINMM}
    ${ENET_INCLUDE_DIR}
    )



add_subdirectory( src )
add_subdirectory( tools )
