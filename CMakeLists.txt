cmake_minimum_required(VERSION 3.8)


option(RMGR_DETECT_BUILD_TESTS  "Whether to build rmgr-detect unit tests" OFF)
if (RMGR_DETECT_BUILD_TESTS)
    set(RMGR_DETECT_LANGUAGES  CXX)
else()
    set(RMGR_DETECT_LANGUAGES  NONE)
endif()

project(rmgr-detect
    VERSION   1.0.0
    LANGUAGES ${RMGR_DETECT_LANGUAGES}
)

include(GNUInstallDirs)

add_library(rmgr-detect INTERFACE)
target_include_directories(rmgr-detect INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")


if (RMGR_DETECT_BUILD_TESTS)
    add_executable(rmgr-detect-tests
        "tests/rmgr-detect-tests.cpp"
        "tests/rmgr-detect-compiler-tests.cpp"
        "tests/rmgr-detect-runtime-tests.cpp"
        "include/rmgr/detect-compiler.h"
        "include/rmgr/detect-runtime.h"
    )
    target_link_libraries(rmgr-detect-tests PRIVATE rmgr-detect)
    set_property(DIRECTORY . PROPERTY VS_STARTUP_PROJECT "rmgr-detect-tests")

    # Identify compiler
    if (MINGW)
        target_link_options(rmgr-detect-tests PRIVATE "-static")
        set(RMGR_DETECT_TESTS_EXPECTED_VARIANT "\"MinGW\"")
    endif()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND         "\"MSVC\"")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND          "\"MSVC\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND         "\"GCC\"")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND          "\"GCC\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_GENERATOR_TOOLSET MATCHES "_clang_c2$")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND         "\"Clang\"")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND          "\"MSVC\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND_VERSION  "\"19.0.0\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND         "\"Clang\"")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND          "\"LLVM\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        include(CheckCXXSymbolExists)
        check_cxx_symbol_exists("__aocc__" "" __aocc__)
        if (__aocc__)
            set(RMGR_DETECT_TESTS_EXPECTED_VARIANT               "\"AOCC\"")
            set(RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION_MAJOR "__aocc_major__")
            set(RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION_MINOR "__aocc_minor__")
            set(RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION_PATCH "__aocc_patchlevel__")
        endif()
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND         "\"EDG\"")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND_VERSION "\"6\"") # TODO: retrieve actual version
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND          "\"ICC\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND         "\"Clang\"")
        set(RMGR_DETECT_TESTS_EXPECTED_FRONTEND_VERSION __clang_version__)
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND          "\"LLVM\"")
        set(RMGR_DETECT_TESTS_EXPECTED_BACKEND_VERSION  __clang_version__)
        set(RMGR_DETECT_TESTS_EXPECTED_VARIANT          "\"ICX\"")
        set(RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
    else()
        message(FATAL_ERROR "Unsupported/unrecognized compiler")
    endif()

    # Identify C runtime library
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(RMGR_DETECT_TESTS_EXPECTED_CRT "\"glibc\"")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(RMGR_DETECT_TESTS_EXPECTED_CRT "\"MSVCRT\"")
    elseif (CYGWIN)
        set(RMGR_DETECT_TESTS_EXPECTED_CRT "\"Newlib\"")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
        set(RMGR_DETECT_TESTS_EXPECTED_CRT "\"Bionic\"")
    else()
        message(FATAL_ERROR "Unsupported/unrecognized platform")
    endif()

    # Forward info to test program
    target_compile_definitions(rmgr-detect-tests PRIVATE
       "EXPECTED_FRONTEND=${RMGR_DETECT_TESTS_EXPECTED_FRONTEND}"
       "EXPECTED_FRONTEND_VERSION=${RMGR_DETECT_TESTS_EXPECTED_FRONTEND_VERSION}"
       "EXPECTED_BACKEND=${RMGR_DETECT_TESTS_EXPECTED_BACKEND}"
       "EXPECTED_BACKEND_VERSION=${RMGR_DETECT_TESTS_EXPECTED_BACKEND_VERSION}"
       "EXPECTED_CRT=${RMGR_DETECT_TESTS_EXPECTED_CRT}"
    )
    if (DEFINED RMGR_DETECT_TESTS_EXPECTED_VARIANT)
        target_compile_definitions(rmgr-detect-tests PRIVATE "EXPECTED_VARIANT=${RMGR_DETECT_TESTS_EXPECTED_VARIANT}")
    endif()
    if (DEFINED RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION)
        target_compile_definitions(rmgr-detect-tests PRIVATE "EXPECTED_VARIANT_VERSION=${RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION}")
    elseif (DEFINED RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION_MAJOR)
        target_compile_definitions(rmgr-detect-tests PRIVATE
            "EXPECTED_VARIANT_VERSION_MAJOR=${RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION_MAJOR}"
            "EXPECTED_VARIANT_VERSION_MINOR=${RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION_MINOR}"
            "EXPECTED_VARIANT_VERSION_PATCH=${RMGR_DETECT_TESTS_EXPECTED_VARIANT_VERSION_PATCH}"
        )
    endif()
endif()
