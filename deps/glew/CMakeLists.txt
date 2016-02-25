set(GLEW_URL
        "https://sourceforge.net/projects/glew/files/glew/1.13.0/glew-1.13.0.tgz/download"
        CACHE STRING "Location of glew source package")

add_custom_target(ext_glew_patch
        SOURCES glew_cmakelists.txt.in)

ExternalProject_Add(ext_glew
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/glew"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/glew"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${GLEW_URL}"
        URL_MD5 "7cbada3166d2aadfc4169c4283701066"
        PATCH_COMMAND
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/glew_cmakelists.txt.in
                    ${EXTERNALS_WORK_DIR}/src/glew/CMakeLists.txt
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            ${TOOLCHAIN_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DBUILD_UTILS=off)
