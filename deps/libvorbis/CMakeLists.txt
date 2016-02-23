set(LIBVORBIS_URL
        "http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.5.tar.gz"
        CACHE STRING "Location of libvorbis source package")

add_custom_target(ext_libvorbis_patch
        SOURCES libvorbis_cmakelists.txt.in)

ExternalProject_Add(ext_libvorbis
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/libvorbis"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/libvorbis"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${LIBVORBIS_URL}"
        URL_HASH SHA256=6efbcecdd3e5dfbf090341b485da9d176eb250d893e3eb378c428a2db38301ce
        PATCH_COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_SOURCE_DIR}/libvorbis_cmakelists.txt.in
                ${EXTERNALS_WORK_DIR}/src/libvorbis/CMakeLists.txt
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_MODULE_PATH=${EXTERNALS_INSTALL_DIR}/lib/cmake)

if(USE_BUNDLED_LIBOGG)
    add_dependencies(ext_libvorbis ext_libogg)
endif()

