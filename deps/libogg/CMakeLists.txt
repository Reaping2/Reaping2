set(LIBOGG_URL
        "http://downloads.xiph.org/releases/ogg/libogg-1.3.2.tar.gz"
        CACHE STRING "Location of libogg source package")

add_custom_target(ext_libogg_patch
        SOURCES libogg_cmakelists.txt.in FindOgg.cmake)

ExternalProject_Add(ext_libogg
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/libogg"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/libogg"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${LIBOGG_URL}"
        URL_HASH SHA256=e19ee34711d7af328cb26287f4137e70630e7261b17cbe3cd41011d73a654692
        PATCH_COMMAND
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/libogg_cmakelists.txt.in
                    ${EXTERNALS_WORK_DIR}/src/libogg/CMakeLists.txt
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/FindOgg.cmake
                    ${EXTERNALS_WORK_DIR}/src/libogg/FindOgg.cmake
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

