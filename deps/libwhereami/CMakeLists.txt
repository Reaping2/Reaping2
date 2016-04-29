set(LIBWHEREAMI_URL
    "https://github.com/gpakosz/whereami/archive/master.zip"
        CACHE STRING "Location of libwhereami source package")

add_custom_target(ext_libwhereami_patch
        SOURCES libwhereami_cmakelists.txt.in)

ExternalProject_Add(ext_libwhereami
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/libwhereami"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/libwhereami"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${LIBWHEREAMI_URL}"
        PATCH_COMMAND
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/libwhereami_cmakelists.txt.in
                    ${EXTERNALS_WORK_DIR}/src/libwhereami/CMakeLists.txt
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            ${TOOLCHAIN_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
