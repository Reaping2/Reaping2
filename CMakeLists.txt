cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(CMAKE_VERBOSE_MAKEFILE ON)

project(Reaping2)
include(CMakeHelper.cmake)

set(BUILD_SHARED_LIBS ON)

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)

link_directories(
	"${PROJECT_SOURCE_DIR}/deps/boost_1_54_0/stage/lib"
	"${PROJECT_SOURCE_DIR}/deps/glfw-3.0.3/build/src/Release"
	"${PROJECT_SOURCE_DIR}/deps/glfw-3.0.3/build/src/Debug"
	"${PROJECT_SOURCE_DIR}/deps/glfw-3.0.3/build/src"
	"${PROJECT_SOURCE_DIR}/deps/zlib-1.2.8/build/Debug"
	"${PROJECT_SOURCE_DIR}/deps/zlib-1.2.8/build/Release"
	"${PROJECT_SOURCE_DIR}/deps/lpng166/build"
	"${PROJECT_SOURCE_DIR}/deps/lpng166/build/Debug"
	"${PROJECT_SOURCE_DIR}/deps/lpng166/build/Release"
	"${PROJECT_SOURCE_DIR}/deps/libogg-1.3.1/build/lib"
	"${PROJECT_SOURCE_DIR}/deps/libogg-1.3.1/lib/debug"
	"${PROJECT_SOURCE_DIR}/deps/libogg-1.3.1/lib/release"
	"${PROJECT_SOURCE_DIR}/deps/libvorbis-1.3.3/bin/lib"
	"${PROJECT_SOURCE_DIR}/deps/libvorbis-1.3.3/bin/lib/debug"
	"${PROJECT_SOURCE_DIR}/deps/libvorbis-1.3.3/bin/lib/release"
	"${PROJECT_SOURCE_DIR}/deps/portaudio_v19/build_dir/bin/Win32/Debug"
	"${PROJECT_SOURCE_DIR}/deps/portaudio_v19/build_dir/bin/Win32/Release"
	"${PROJECT_SOURCE_DIR}/deps/portaudio_v19/build_dir/lib"
	"${PROJECT_SOURCE_DIR}/deps/glew-1.10.0/lib")

link_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/bin/libs")

include_directories(
	"${PROJECT_SOURCE_DIR}/src"
	"${PROJECT_SOURCE_DIR}/deps/boost_1_54_0"
	"${PROJECT_SOURCE_DIR}/deps/glfw-3.0.3/include"
	"${PROJECT_SOURCE_DIR}/deps/glm_0_9_4_6"
	"${PROJECT_SOURCE_DIR}/deps/zlib-1.2.8"
	"${PROJECT_SOURCE_DIR}/deps/zlib-1.2.8/build"
	"${PROJECT_SOURCE_DIR}/deps/lpng166"
	"${PROJECT_SOURCE_DIR}/deps/json-cpp-0.6.0-rc2"
	"${PROJECT_SOURCE_DIR}/deps/libogg-1.3.1/include"
	"${PROJECT_SOURCE_DIR}/deps/libvorbis-1.3.3/include"
	"${PROJECT_SOURCE_DIR}/deps/portaudio_v19/build_dir/include"
	"${PROJECT_SOURCE_DIR}/deps/portaudio_v19/include"
	"${PROJECT_SOURCE_DIR}/deps/glew-1.10.0/include")



add_subdirectory(src)

