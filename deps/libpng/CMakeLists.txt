set(LIBPNG_URL
        "http://prdownloads.sourceforge.net/libpng/libpng-1.6.21.tar.gz"
        CACHE STRING "Location of libpng source package")

ExternalProject_Add(ext_libpng
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/libpng"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/libpng"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL ${LIBPNG_URL}
        URL_MD5 aca36ec8e0a3b406a5912243bc243717
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            ${TOOLCHAIN_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

if(USE_BUNDLED_ZLIB)
    add_dependencies(ext_libpng ext_zlib)
endif()

