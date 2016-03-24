set(LIBDBG_URL
    "https://bitbucket.org/edd/dbg/get/0.2.1.tar.gz"
    CACHE STRING "Location of dbg source package")

ExternalProject_Add(ext_dbg
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/dbg"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/dbg"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${LIBDBG_URL}"
        URL_HASH SHA256=3c683d92c5a5b0a005d54257178512188d32d1affb7533e175f829d34e2e8c2a
        PATCH_COMMAND
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/dbg_CMakeLists.txt
                    ${EXTERNALS_WORK_DIR}/src/dbg/CMakeLists.txt
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/FindDbg.cmake
                    ${EXTERNALS_WORK_DIR}/src/dbg/FindDbg.cmake
            COMMAND ${CMAKE_COMMAND} -E copy
                    ${CMAKE_CURRENT_SOURCE_DIR}/frames.cpp
                    ${EXTERNALS_WORK_DIR}/src/dbg/src/windows/frames.cpp
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            ${TOOLCHAIN_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})

