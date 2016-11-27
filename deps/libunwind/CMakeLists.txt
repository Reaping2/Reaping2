set(LIBUNWIND_URL
        "http://download.savannah.gnu.org/releases/libunwind/libunwind-1.2-rc1.tar.gz"
        CACHE STRING "Location of libunwind source package")

ExternalProject_Add(ext_libunwind
        PREFIX "${EXTERNALS_WORK_DIR}"
        SOURCE_DIR "${EXTERNALS_WORK_DIR}/src/libunwind"
        BINARY_DIR "${EXTERNALS_WORK_DIR}/build/libunwind"
        INSTALL_DIR "${EXTERNALS_INSTALL_DIR}"
        URL "${LIBUNWIND_URL}"
        URL_HASH SHA256=d222f186b6bc60f49dac5030516ec35a7ed0ccca675551d6cf81008112116abc
        CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --libdir=<INSTALL_DIR>/lib
        BUILD_COMMAND make
        INSTALL_COMMAND make install
    )

