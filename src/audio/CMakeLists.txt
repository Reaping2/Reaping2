cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
get_filename_component(dirnam ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB ${dirnam}_source_files ./ *.cpp *.h)

add_library(${dirnam} STATIC ${${dirnam}_source_files})
add_definitions(-DSUBSYSTEM="${dirnam}")
target_link_libraries(${dirnam} ${PORTAUDIO_LIBRARIES} ${LIBVORBIS_LIBRARIES} ${LIBOGG_LIBRARIES})

add_precompiled_header(${dirnam} pch.h)

INSTALL(TARGETS ${dirnam}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)

