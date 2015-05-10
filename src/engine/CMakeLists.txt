cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
get_filename_component(dirnam ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB ${dirnam}_source_files ./ *.cpp *.h)

add_library(${dirnam} STATIC ${${dirnam}_source_files})
target_link_libraries(${dirnam} platform core controllers items collisions buffs_engine render input network)

add_precompiled_header(${dirnam} pch.h)
add_subdirectory(controllers)
add_subdirectory(collisions)
add_subdirectory(items)
add_subdirectory(buffs_engine)

INSTALL(TARGETS ${dirnam}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)
