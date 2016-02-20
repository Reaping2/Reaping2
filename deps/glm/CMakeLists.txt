set(GLM_URL
        "https://github.com/g-truc/glm/archive/0.9.7.2.tar.gz"
        CACHE STRING "Location of glm source package")

ExternalProject_Add(ext_glm
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/glm"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/glm"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${GLM_URL}"
        URL_MD5 "e1aa3fdbbbe0edc924165cc5f0bc24f6"
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

