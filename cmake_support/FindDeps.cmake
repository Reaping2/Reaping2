# win32: set BOOST_LIBRARYDIR and BOOST_INCLUDEDIR folders
if(WIN32)
	# ugly boost bug workaround, boost w/ static build crashes in codecvt
	set( Boost_USE_STATIC_LIBS ON )
endif(WIN32)
find_package( Boost 1.51 COMPONENTS filesystem system thread date_time program_options atomic )

if( NOT Boost_FOUND )
	message( FATAL_ERROR "Cannot find Boost!" )
endif()

find_package( OpenGL )

# add possible lib names at the end of the arg list
macro( my_find_package pkg_name header default_lib_name )
	if(WIN32)
		set( lib_names ${ARGN})
		find_path( ${pkg_name}_INCLUDE_DIR ${header} PATHS $ENV{${pkg_name}_INCLUDEDIR} )
		find_library( ${pkg_name}_LIBRARY NAMES ${default_lib_name} ${lib_names} PATHS $ENV{${pkg_name}_LIBRARYDIR} )
		set( ${pkg_name}_INCLUDE_DIRS ${${pkg_name}_INCLUDE_DIR} CACHE PATH "" FORCE )
		set( ${pkg_name}_LIBRARIES ${${pkg_name}_LIBRARY} CACHE PATH "" FORCE )
		message( "${pkg_name}: ${${pkg_name}_INCLUDE_DIR} ${${pkg_name}_LIBRARIES}" )
	else(WIN32)
		find_package( ${pkg_name} )
		if(NOT ${${pkg_name}_FOUND} )
			message( "${pkg_name} not found! Default: ${default_lib_name} " )
			set( ${pkg_name}_LIBRARY ${default_lib_name} CACHE PATH "" FORCE )
			set( ${pkg_name}_INCLUDE_DIR "" CACHE PATH "" FORCE )
			set( ${pkg_name}_INCLUDE_DIRS ${${pkg_name}_INCLUDE_DIR} CACHE PATH "" FORCE )
			set( ${pkg_name}_LIBRARIES ${${pkg_name}_LIBRARY} CACHE PATH "" FORCE )
		else()
#			message( "${pkg_name} found!" )
#			if(NOT ${pkg_name}_INCLUDE_DIRS )
#				set( ${pkg_name}_INCLUDE_DIRS ${${pkg_name}_INCLUDE_DIR} CACHE PATH "" FORCE )
#			endif()
#			if(NOT ${pkg_name}_LIBRARIES )
#				set( ${pkg_name}_LIBRARIES ${${pkg_name}_LIBRARY} CACHE PATH "" FORCE )
#			endif()
		endif()
		message( "${pkg_name}: ${${pkg_name}_INCLUDE_DIR} ${${pkg_name}_LIBRARY}" )
		
	endif(WIN32)
endmacro( my_find_package )

my_find_package( GLEW GL/glew.h GLEW glew32 glew glew32s )

my_find_package( GLFW GLFW/glfw3.h glfw glfw3 glfw3dll )

if(WIN32)
	find_path( GLM_INCLUDE_DIR glm/glm.hpp PATHS $ENV{GLM_INCLUDEDIR} )
	message( "GLM: ${GLM_INCLUDE_DIR}" )
	string( COMPARE EQUAL ${GLM_INCLUDE_DIR} "GLM_INCLUDE_DIR-NOTFOUND" glm_missing )
	if( ${glm_missing} )
		message( FATAL_ERROR "GLM not found ")
	endif()
else()
	set( GLM_INCLUDE_DIR "" )
endif()

my_find_package( LIBOGG ogg/ogg.h ogg libogg )

my_find_package( LIBVORBIS vorbis/vorbisfile.h vorbis libvorbis )
if(WIN32)
	find_library( temp NAMES libvorbisfile PATHS $ENV{LIBVORBIS_LIBRARYDIR})
	set( LIBVORBIS_LIBRARIES ${LIBVORBIS_LIBRARIES} ${temp} )
else()
	set( LIBVORBIS_LIBRARIES ${LIBVORBIS_LIBRARIES} vorbisfile )
endif()

my_find_package( ZLIB zlib.h z zdll zlib )

my_find_package( PNG png.h png libpng16 libpng16_static libpng16d libpng16_staticd )

my_find_package( PORTAUDIO portaudio.h portaudio portaudio_x86 portaudio_static_x86 portaudio_x64 portaudio_static_x64 )

my_find_package( ENET enet/enet.h enet enet64 enet.lib enet64.lib )

if(WIN32)
    set( PLATFORM_LIBS "" )
else()
    set( PLATFORM_LIBS pthread rt X11 Xrandr Xi Xxf86vm )
endif()

set( PORTAUDIO_CMAKE_INCLUDE_DIR $ENV{PORTAUDIO_CMAKE_INCLUDEDIR} )
