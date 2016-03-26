cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
get_filename_component(dirnam ${CMAKE_CURRENT_LIST_DIR} NAME)

file(GLOB_RECURSE ${dirnam}_source_files ./ *.cpp *.h)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
add_library(${dirnam} STATIC ${${dirnam}_source_files} ${CMAKE_SOURCE_DIR}/deps/json-cpp-0.6.0-rc2/jsoncpp.cpp)
set( lib_deps ${Boost_LIBRARIES} ${PNG_LIBRARIES} ${ZLIB_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${PLATFORM_LIBS} ${ENET_LIBRARIES} )
target_link_libraries(${dirnam} ${lib_deps})
if( WIN32 )
    target_link_libraries(${dirnam} ${DBG_LIBRARY})
else( WIN32 )
    target_link_libraries(${dirnam} unwind)
endif( WIN32 )

add_precompiled_header(${dirnam} pch.h)

INSTALL(TARGETS ${dirnam}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	)

