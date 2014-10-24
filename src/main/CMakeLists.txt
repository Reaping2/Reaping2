cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
get_filename_component(dirnam ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB ${dirnam}_source_files ./ *.cpp *.h)

add_executable(${dirnam} ${${dirnam}_source_files} )
add_dependencies( main audio input platform core render)
target_link_libraries( main audio input platform core render ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} )

add_precompiled_header(${dirnam} pch.h)

INSTALL(TARGETS ${dirnam}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)

