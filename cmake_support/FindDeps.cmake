# win32: set BOOST_LIBRARYDIR and BOOST_INCLUDEDIR folders
if(WIN32)
	# ugly boost bug workaround, boost w/ static build crashes in codecvt
	set( Boost_USE_STATIC_LIBS ON )
        if(MINGW)
            add_definitions( -DBOOST_THREAD_USE_LIB )
        endif(MINGW)
endif(WIN32)

set(Boost_ADDITIONAL_VERSIONS 1.59 1.60)

find_package( Boost 1.60 EXACT REQUIRED
        COMPONENTS filesystem system thread date_time program_options atomic serialization chrono )

find_package( OpenGL REQUIRED )

# add possible lib names at the end of the arg list
macro( my_find_package pkg_name header default_lib_name )

    find_package( ${pkg_name} QUIET CONFIG )

    string(TOUPPER ${pkg_name} pkg_name_upper)

    if(${${pkg_name}_FOUND})
        if(DEFINED ${pkg_name_upper}_LIBRARY AND NOT DEFINED ${pkg_name_upper}_LIBRARIES)
            set(${pkg_name_upper}_LIBRARIES ${${pkg_name_upper}_LIBRARY})
        endif(DEFINED ${pkg_name_upper}_LIBRARY AND NOT DEFINED ${pkg_name_upper}_LIBRARIES)
        message( STATUS "${pkg_name_upper} found!" )
    else(${${pkg_name}_FOUND})
        message(STATUS "Package ${pkg_name_upper} not found! Trying to find paths manually ...")

	    if( NOT "${header}" STREQUAL "" )
            find_path(${pkg_name_upper}_INCLUDE_DIR ${header}
                PATHS
                    ${REAPING2_DEPS_INSTALL_DIR}/include
                    $ENV{${pkg_name_upper}_INCLUDEDIR})
            if(NOT ${pkg_name_upper}_INCLUDE_DIR)
                message(FATAL_ERROR "${pkg_name_upper} include dir not found! Searched for: ${header}" )
	        endif(NOT ${pkg_name_upper}_INCLUDE_DIR)
	    endif( NOT "${header}" STREQUAL "" )

	    if( NOT "${default_lib_name}" STREQUAL "" )
            set(lib_names ${ARGN})
            find_library(${pkg_name_upper}_LIBRARY
                NAMES ${default_lib_name} ${lib_names}
                PATHS
                    ${REAPING2_DEPS_INSTALL_DIR}/lib
                    $ENV{${pkg_name_upper}_LIBRARYDIR})
            if(NOT ${pkg_name}_LIBRARY)
                message(FATAL_ERROR "${pkg_name_upper} library not found! Searched for: ${default_lib_name};${lib_names}")
            endif()
	    endif( NOT "${default_lib_name}" STREQUAL "" )

        set( ${pkg_name_upper}_INCLUDE_DIRS ${${pkg_name_upper}_INCLUDE_DIR} CACHE PATH "" FORCE )
        set( ${pkg_name_upper}_LIBRARIES ${${pkg_name_upper}_LIBRARY} CACHE PATH "" FORCE )

        message( STATUS "${pkg_name_upper} include dir: \"${${pkg_name_upper}_INCLUDE_DIR}\" library: \"${${pkg_name_upper}_LIBRARY}\"" )
    endif(${${pkg_name}_FOUND})

endmacro( my_find_package )

set(GLEW_USE_STATIC_LIBS ON)
find_package(glew QUIET CONFIG)
if(glew_FOUND)
    message(STATUS "GLEW found!")
    # Name of imported target
    set(GLEW_LIBRARIES GLEW::GLEW)
else(glew_FOUND)
    my_find_package( GLEW GL/glew.h GLEW glew32s glew32sd )
endif(glew_FOUND)

my_find_package( glfw GLFW/glfw3.h glfw glfw3 glfw3dll )

my_find_package( glm glm/glm.hpp "" )

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
