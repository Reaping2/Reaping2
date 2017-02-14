cmake_minimum_required(VERSION 2.8)

find_path(DBG_INCLUDE_DIR dbg/debugger.hpp)

set(DBG_NAMES ${DBG_NAMES} dbg libdbg dbgd libdbgd)
find_library(DBG_LIBRARY NAMES ${DBG_NAMES} PATH)

if(DBG_INCLUDE_DIR AND DBG_LIBRARY)
    set(DBG_FOUND TRUE)
endif()

if(DBG_FOUND)
    if(NOT Dbg_FIND_QUIETLY)
        message(STATUS "Found dbg: ${DBG_LIBRARY}")
    endif()
else(DBG_FOUND)
    if(Dbg_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find dbg")
    endif()
endif(DBG_FOUND)
