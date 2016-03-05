# the name of the target operating system
SET(CMAKE_SYSTEM_NAME Windows)

# which compilers to use for C and C++
SET(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
SET(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# here is the target environment located
SET(CMAKE_FIND_ROOT_PATH  /usr/x86_64-w64-mingw32 ${CMAKE_FIND_ROOT_PATH})
LIST(REMOVE_DUPLICATES CMAKE_FIND_ROOT_PATH)

# adjust the default behaviour of the FIND_XXX() commands:
# search headers and libraries in the target environment, search 
# programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(Boost_THREADAPI win32)

set(CMAKE_TOOLCHAIN_BOOST_TOOLSET_ARGS "toolset=gcc-mingw" "target-os=windows" "threadapi=win32" "threading=multi" "architecture=x86" "variant=debug" "--user-config=${CMAKE_CURRENT_LIST_DIR}/user-config.jam")
message( STATUS "Using toolset in ${CMAKE_CURRENT_LIST_DIR}" )
message( STATUS "Root path ${CMAKE_FIND_ROOT_PATH}" )
