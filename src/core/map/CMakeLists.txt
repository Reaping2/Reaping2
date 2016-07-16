cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
get_filename_component(dirnam ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB ${dirnam}_source_files ./ *.cpp *.h)

add_library(${dirnam} STATIC ${${dirnam}_source_files})
target_link_libraries(${dirnam} engine input level_generator)

add_precompiled_header(${dirnam} pch.h)
add_subdirectory(level_generator)

INSTALL(TARGETS ${dirnam}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)
