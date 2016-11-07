add_executable( aicollector main.cpp ${CMAKE_SOURCE_DIR}/deps/json-cpp-0.6.0-rc2/jsoncpp.cpp )
target_link_libraries(aicollector  
	${Boost_LIBRARIES}
	)

set_property(TARGET aicollector PROPERTY FOLDER "tools")
