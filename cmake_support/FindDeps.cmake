# win32: set BOOST_LIBRARYDIR and BOOST_INCLUDEDIR folders
if(WIN32)
	# ugly boost bug workaround, boost w/ static build crashes in codecvt
	set( Boost_USE_STATIC_LIBS ON )
        if(MINGW)
            add_definitions( -DBOOST_THREAD_USE_LIB )
        endif(MINGW)
endif(WIN32)
set(Boost_ADDITIONAL_VERSIONS 1.59 1.60)
find_package( Boost 1.59 COMPONENTS filesystem system thread date_time program_options atomic serialization chrono )

if( NOT Boost_FOUND )
	message( FATAL_ERROR "Cannot find Boost!" )
endif()

find_package( OpenGL )

# add possible lib names at the end of the arg list
macro( my_find_package pkg_name header default_lib_name )

    find_package( ${pkg_name} QUIET MODULE )

    if(NOT ${${pkg_name}_FOUND})
        message(STATUS "Package ${pkg_name} not found! Trying to find paths manually ...")

	    if( NOT "${header}" STREQUAL "" )
            find_path(${pkg_name}_INCLUDE_DIR ${header}
                PATHS
                    ${REAPING2_DEPS_INSTALL_DIR}/include
                    $ENV{${pkg_name}_INCLUDEDIR})
            if(NOT ${pkg_name}_INCLUDE_DIR)
                message(FATAL_ERROR "${pkg_name} include dir not found! Searched for: ${header}" )
	        endif(NOT ${pkg_name}_INCLUDE_DIR)
	    endif( NOT "${header}" STREQUAL "" )

	    if( NOT "${default_lib_name}" STREQUAL "" )
            set(lib_names ${ARGN})
            find_library(${pkg_name}_LIBRARY
                NAMES ${default_lib_name} ${lib_names}
                PATHS
                    ${REAPING2_DEPS_INSTALL_DIR}/lib
                    $ENV{${pkg_name}_LIBRARYDIR})
            if(NOT ${pkg_name}_LIBRARY)
                message(FATAL_ERROR "${pkg_name} library not found! Searched for: ${default_lib_name};${lib_names}")
            endif()
	    endif( NOT "${default_lib_name}" STREQUAL "" )

        set( ${pkg_name}_INCLUDE_DIRS ${${pkg_name}_INCLUDE_DIR} CACHE PATH "" FORCE )
        set( ${pkg_name}_LIBRARIES ${${pkg_name}_LIBRARY} CACHE PATH "" FORCE )

        message( STATUS "${pkg_name} include dir: \"${${pkg_name}_INCLUDE_DIR}\" library: \"${${pkg_name}_LIBRARY}\"" )
    else()
        message( STATUS "${pkg_name} found!" )
    endif()


endmacro( my_find_package )

my_find_package( GLEW GL/glew.h GLEW glew32s glew32sd )

my_find_package( GLFW GLFW/glfw3.h glfw glfw3 glfw3dll )

if(WIN32)
    my_find_package( GLM glm/glm.hpp "" )
else()
    set( glm_include_dir "" )
endif()

my_find_package( LIBOGG ogg/ogg.h ogg libogg libogg_static )

my_find_package( LIBVORBIS vorbis/vorbisfile.h vorbis ${VORBISFILE_LIBRARY_NAMES} )
my_find_package( LIBVORBISFILE "" vorbisfile vorbisfile_static )
set(LIBVORBIS_LIBRARIES ${LIBVORBIS_LIBRARY} ${LIBVORBISFILE_LIBRARY})

my_find_package( ZLIB zlib.h z zdll zlib zlibd)

my_find_package( PNG png.h png libpng16 libpng16_static libpng16d libpng16_staticd )

my_find_package( PORTAUDIO portaudio.h portaudio portaudio_x86 portaudio_static_x86 portaudio_x64 portaudio_static_x64 )

my_find_package( ENET enet/enet.h enet enet64 enet.lib enet64.lib )

if(WIN32)
    set( PLATFORM_LIBS "" )
else()
    set( PLATFORM_LIBS pthread rt X11 Xrandr Xi Xxf86vm )
endif()

set( PORTAUDIO_CMAKE_INCLUDE_DIR $ENV{PORTAUDIO_CMAKE_INCLUDEDIR} )
