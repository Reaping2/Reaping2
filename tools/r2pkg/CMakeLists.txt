add_executable( r2pkg main.cpp )
target_link_libraries( r2pkg
	${Boost_LIBRARIES}
	platform
	)

set_property(TARGET r2pkg PROPERTY FOLDER "tools")
