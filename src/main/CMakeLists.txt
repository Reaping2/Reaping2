cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
get_filename_component(dirnam ${CMAKE_CURRENT_LIST_DIR} NAME)

# git version checking:
# https://cmake.org/pipermail/cmake/2010-July/038015.html
# Michael Hertling
find_package(Git)
file(WRITE ${CMAKE_BINARY_DIR}/version.h.in
    "\#define GIT_VERSION \"@VERSION@\"\n#define GIT_DATE \"@DATE@\"\n#define GIT_BRANCH \"@BRANCH@\"\n#define GIT_REMOTE \"@REMOTE@\"\n"
    )
file(WRITE ${CMAKE_BINARY_DIR}/version.cmake
    "EXECUTE_PROCESS(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
EXECUTE_PROCESS(
    COMMAND ${GIT_EXECUTABLE} log -n 1 --format=%ai
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE DATE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
EXECUTE_PROCESS(
    COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
EXECUTE_PROCESS(
    COMMAND ${GIT_EXECUTABLE} ls-remote --get-url origin
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE REMOTE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
CONFIGURE_FILE(\${SRC} \${DST} @ONLY)
")
add_custom_target(
    version
    ${CMAKE_COMMAND} -D SRC=${CMAKE_BINARY_DIR}/version.h.in
    -D DST=${CMAKE_BINARY_DIR}/version.h
    -P ${CMAKE_BINARY_DIR}/version.cmake
    )
include_directories( BEFORE ${CMAKE_BINARY_DIR} )

file(GLOB ${dirnam}_source_files ./ *.cpp *.h)

add_executable(${dirnam} ${${dirnam}_source_files} )
add_dependencies( main audio input platform core render version)
target_link_libraries( main audio input platform core engine render network ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${WIN32_ENET_INCLUDE_WS2_32} ${WIN32_ENET_INCLUDE_WINMM} ${ENET_LIBRARIES} )
add_precompiled_header(${dirnam} pch.h)

INSTALL(TARGETS ${dirnam}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)

