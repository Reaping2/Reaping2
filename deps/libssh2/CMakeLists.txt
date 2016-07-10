set(LIBSSH2_URL
    "https://www.libssh2.org/download/libssh2-1.7.0.tar.gz"
    CACHE STRING "Location of libssh2 source package")

if(UNIX)
    set(LIBSSH2_BUILD_SHARED_DEFAULT ON)
else(UNIX)
    set(LIBSSH2_BUILD_SHARED_DEFAULT OFF)
endif(UNIX)

option(LIBSSH2_BUILD_SHARED "Build shared libssh2 libs" ${LIBSSH2_BUILD_SHARED_DEFAULT})

ExternalProject_Add(ext_libssh2
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/libssh2"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/libssh2"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${LIBSSH2_URL}"
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            ${TOOLCHAIN_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DBUILD_SHARED_LIBS=${LIBSSH2_BUILD_SHARED}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DBUILD_EXAMPLES=OFF
            -DBUILD_TESTING=OFF
        )

