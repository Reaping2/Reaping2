set(ENET_URL
        "http://enet.bespin.org/download/enet-1.3.13.tar.gz"
        CACHE STRING "Location of enet source package")

set(ENET_LIBRARY ${CMAKE_STATIC_LIBRARY_PREFIX}enet${CMAKE_STATIC_LIBRARY_SUFFIX})

ExternalProject_Add(ext_enet
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/enet"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/enet"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${ENET_URL}"
        URL_MD5 b83b9a7791417b6b6f5c68231f6e218b
        CMAKE_ARGS
            ${PLATFORM_CMAKE_ARGS}
            -G ${CMAKE_GENERATOR}
            -DCMAKE_INSTALL_PREFIX=${EXTERNALS_INSTALL_DIR}
            -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
            -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG=${PROJECT_BINARY_DIR}
            -DCMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE=${EXTERNALS_WORK_DIR}/build/enet
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG=${EXTERNALS_WORK_DIR}/build/enet
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE=${EXTERNALS_WORK_DIR}/build/enet
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG=${EXTERNALS_WORK_DIR}/build/enet
            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE=${EXTERNALS_WORK_DIR}/build/enet
        INSTALL_COMMAND
            COMMAND ${CMAKE_COMMAND} -E copy
                ${EXTERNALS_WORK_DIR}/build/enet/${ENET_LIBRARY}
                ${EXTERNALS_INSTALL_DIR}/lib/${ENET_LIBRARY}
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${EXTERNALS_WORK_DIR}/src/enet/include/enet
                ${EXTERNALS_INSTALL_DIR}/include/enet)

