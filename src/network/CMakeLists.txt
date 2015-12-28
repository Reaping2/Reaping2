cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
get_filename_component(dirnam ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB ${dirnam}_source_files ./ *.cpp *.h)
add_library(${dirnam} STATIC ${${dirnam}_source_files})
target_link_libraries(${dirnam} platform core engine ${Boost_LIBRARIES} ${WIN32_ENET_INCLUDE_WS2_32} ${WIN32_ENET_INCLUDE_WINMM} ${ENET_LIBRARIES} )
set_property(TARGET ${dirnam} PROPERTY CXX_STANDARD 11)

add_precompiled_header(${dirnam} pch.h)

INSTALL(TARGETS ${dirnam}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)

