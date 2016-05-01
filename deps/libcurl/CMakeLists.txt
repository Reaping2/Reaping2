set(LIBCURL_URL
    "https://github.com/curl/curl/archive/curl-7_48_0.zip"
    CACHE STRING "Location of libcurl source package")

add_custom_target(ext_libcurl_patch
        SOURCES curl_cmakelists.txt.in)

ExternalProject_Add(ext_libcurl
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/libcurl"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/libcurl"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${LIBCURL_URL}"
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/curl_cmakelists.txt.in
                ${EXTERNALS_WORK_DIR}/src/libcurl/CMakeLists.txt
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            ${TOOLCHAIN_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

