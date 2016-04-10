set(BOOST_URL
        "http://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.gz"
        CACHE STRING "Location of boost source package")

if(CMAKE_HOST_WIN32)
    set(BOOST_BOOTSTRAP_CMD bootstrap.bat)
    set(BOOST_BUILD_CMD b2)
    if("v90" STREQUAL "${CMAKE_GENERATOR_TOOLSET}")
        set(BOOST_TOOLSET_ARG "--toolset=msvc-9.0")
    elseif("v100" STREQUAL "${CMAKE_GENERATOR_TOOLSET}")
        set(BOOST_TOOLSET_ARG "--toolset=msvc-10.0")
    elseif("v110" STREQUAL "${CMAKE_GENERATOR_TOOLSET}")
        set(BOOST_TOOLSET_ARG "--toolset=msvc-11.0")
    elseif("v120" STREQUAL "${CMAKE_GENERATOR_TOOLSET}")
        set(BOOST_TOOLSET_ARG "--toolset=msvc-12.0")
    elseif("v140" STREQUAL "${CMAKE_GENERATOR_TOOLSET}")
        set(BOOST_TOOLSET_ARG "--toolset=msvc-14.0")
    endif()
    if(MINGW)
        set(BOOST_TOOLSET_ARG "--toolset=gcc")
    endif(MINGW)
else(CMAKE_HOST_WIN32)
    set(BOOST_BOOTSTRAP_CMD ./bootstrap.sh)
    set(BOOST_BUILD_CMD ./b2)
    if(WIN32 AND CMAKE_CROSSCOMPILING AND CMAKE_TOOLCHAIN_BOOST_TOOLSET_ARGS)
        set(BOOST_TOOLSET_ARG ${CMAKE_TOOLCHAIN_BOOST_TOOLSET_ARGS})
    else(WIN32 AND CMAKE_CROSSCOMPILING AND CMAKE_TOOLCHAIN_BOOST_TOOLSET_ARGS)
        set(BOOST_RPATH_ARG "dll-path=${EXTERNALS_INSTALL_DIR}/lib")
    endif(WIN32 AND CMAKE_CROSSCOMPILING AND CMAKE_TOOLCHAIN_BOOST_TOOLSET_ARGS)
endif(CMAKE_HOST_WIN32)

if(NOT DEFINED BOOST_ADDRESS_MODEL)
    # This script does not handle all cases on all platforms,
    # but what could possibly go wrong anyway

    include(ArchDetect)

    detect_architecture(COMPILERARCH)
    string(LENGTH ${COMPILERARCH} ARCHSTRLEN)
    math(EXPR SUBSTRBEGIN ${ARCHSTRLEN}-2)
    string(SUBSTRING ${COMPILERARCH} ${SUBSTRBEGIN} 2 ARCH_END)

    if("64" STREQUAL "${ARCH_END}")
        set(BOOST_ADDRESS_MODEL_ARG "address-model=64")
    else()
        set(BOOST_ADDRESS_MODEL_ARG "address-model=32")
    endif()
else()
    set(BOOST_ADDRESS_MODEL_ARG "address-model=${BOOST_ADDRESS_MODEL}")
endif()


ExternalProject_Add(ext_boost
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/boost"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${BOOST_URL}"
        URL_MD5 "28f58b9a33469388302110562bdf6188"
        BUILD_IN_SOURCE 1
        CONFIGURE_COMMAND ${BOOST_BOOTSTRAP_CMD}
        BUILD_COMMAND ${BOOST_BUILD_CMD}
                        --build-dir="${EXTERNALS_WORK_DIR}/build/boost"
                        --with-filesystem
                        --with-system
                        --with-exception
                        --with-system
                        --with-thread
                        --with-date_time
                        --with-program_options
                        --with-atomic
                        --with-serialization
                        --with-chrono
                        --build-dir=${EXTERNALS_WORK_DIR}/build/boost
                        --prefix=${EXTERNALS_INSTALL_DIR}
                        --includedir=${EXTERNALS_INSTALL_DIR}/include
                        ${BOOST_ADDRESS_MODEL_ARG}
                        ${BOOST_TOOLSET_ARG}
                        ${BOOST_RPATH_ARG}
                        install
        INSTALL_COMMAND "")

