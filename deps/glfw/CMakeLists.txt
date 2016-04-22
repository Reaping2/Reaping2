set(GLFW_URL
        "http://sourceforge.net/projects/glfw/files/glfw/3.0.4/glfw-3.0.4.tar.gz/download"
        CACHE STRING "Location of glfw source package")

if(UNIX)
    set(GLFW_BUILD_SHARED_DEFAULT ON)
else(UNIX)
    set(GLFW_BUILD_SHARED_DEFAULT OFF)
endif(UNIX)

option(GLFW_BUILD_SHARED "Build shared glfw libs" ${GLFW_BUILD_SHARED_DEFAULT})

ExternalProject_Add(ext_glfw
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/glfw"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/glfw"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        GIT_REPOSITORY "https://github.com/glfw/glfw.git"
        GIT_TAG "d97044d9acc0c76433edda5a75b3196957906ca5"
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            ${TOOLCHAIN_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DBUILD_SHARED_LIBS=${GLFW_BUILD_SHARED}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

