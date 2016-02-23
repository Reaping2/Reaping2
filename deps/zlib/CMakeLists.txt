set(ZLIB_URL
        "http://zlib.net/zlib-1.2.8.tar.gz"
        CACHE STRING "Location of zlib source package")

ExternalProject_Add(ext_zlib
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/zlib"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/zlib"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${ZLIB_URL}"
        URL_MD5 44d667c142d7cda120332623eab69f40
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

