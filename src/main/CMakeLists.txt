cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
get_filename_component(dirnam ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB ${dirnam}_source_files ./ *.cpp *.h)

add_executable(${dirnam} ${${dirnam}_source_files} ${CMAKE_SOURCE_DIR}/deps/json-cpp-0.6.0-rc2/jsoncpp.cpp )
add_dependencies(main audio input platform core render)
target_link_libraries(main audio input platform core render)

add_library(boost_atomic STATIC IMPORTED)
set_target_properties(boost_atomic PROPERTIES
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bin/libs/libboost_atomic.so)

add_library(boost_filesystem STATIC IMPORTED)
set_target_properties(boost_filesystem PROPERTIES
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bin/libs/libboost_filesystem.so)


add_library(boost_system STATIC IMPORTED)
set_target_properties(boost_system PROPERTIES
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bin/libs/libboost_system.so)


add_library(boost_thread STATIC IMPORTED)
set_target_properties(boost_thread PROPERTIES
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bin/libs/libboost_thread.so)

add_library(GLEW STATIC IMPORTED)
set_target_properties(GLEW PROPERTIES
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bin/libs/libGLEW.so)

add_library(ogg STATIC IMPORTED)
set_target_properties(ogg PROPERTIES
	IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/bin/libs/libogg.so)




target_link_libraries(main
	boost_system
	boost_filesystem
	boost_thread
	GLEW
	ogg)


add_precompiled_header(${dirnam} pch.h)

INSTALL(TARGETS ${dirnam}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)

