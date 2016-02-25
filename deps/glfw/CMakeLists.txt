set(GLFW_URL
        "http://sourceforge.net/projects/glfw/files/glfw/3.0.4/glfw-3.0.4.tar.gz/download"
        CACHE STRING "Location of glfw source package")

ExternalProject_Add(ext_glfw
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/glfw"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/glfw"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${GLFW_URL}"
        URL_MD5 "1e82fdefb1b0df2727cce116d21cd8af"
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            ${TOOLCHAIN_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

