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
        GIT_TAG "999f3556fdd80983b10051746264489f2cb1ef16"
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            ${TOOLCHAIN_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DBUILD_SHARED_LIBS=${GLFW_BUILD_SHARED}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

