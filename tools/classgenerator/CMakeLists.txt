add_executable( classgenerator main.cpp )
target_link_libraries( classgenerator
	${Boost_LIBRARIES}
	platform
	)

set_property(TARGET classgenerator PROPERTY FOLDER "tools")
