SET(ARCH_C_SOURCE "
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
    #error ArchDetect x86_64
#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
    #error ArchDetect ia64
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
    #error ArchDetect i386
#endif

#error cmake_ARCH unkown
")

FUNCTION(DETECT_ARCHITECTURE OutVar)
    ENABLE_LANGUAGE(C)
    FILE(WRITE "${CMAKE_BINARY_DIR}/arch.c" "${ARCH_C_SOURCE}")
    TRY_RUN(
        run_result_unused
        compile_result_unused
        "${CMAKE_BINARY_DIR}"
        "${CMAKE_BINARY_DIR}/arch.c"
        COMPILE_OUTPUT_VARIABLE ARCH)
        string(REGEX MATCH "ArchDetect ([a-zA-Z0-9_]+)" ARCH "${ARCH}")
        string(REPLACE "ArchDetect " "" ARCH "${ARCH}")

    SET(${OutVar} "${ARCH}" PARENT_SCOPE)
endfunction()

