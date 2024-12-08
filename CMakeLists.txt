cmake_minimum_required(VERSION 3.5...3.27)

if (PROJECT_NAME)
    set(RMGR_ID_IS_TOP_LEVEL 0)
else()
    set(RMGR_ID_IS_TOP_LEVEL 1)
endif()

option(RMGR_ID_BUILD_TESTS  "Whether to build rmgr-identify unit tests" ${RMGR_ID_IS_TOP_LEVEL})
if (RMGR_ID_BUILD_TESTS)
    set(RMGR_ID_LANGUAGES  C CXX)
    include(CheckLanguage)
    check_language(ISPC)
    if (CMAKE_ISPC_COMPILER)
        list(APPEND RMGR_ID_LANGUAGES ISPC)
    endif()
else()
    set(RMGR_ID_LANGUAGES  NONE)
endif()

if (RMGR_ID_IS_TOP_LEVEL AND CMAKE_GENERATOR_TOOLSET MATCHES "_clang_c2$")
    set(CMAKE_CXX_FLAGS_DEBUG "/Z7")
    #add_compile_options("$<$<CONFIG:Debug>:/Z7")
endif()


project(rmgr-identify
    VERSION   1.0.0
    LANGUAGES ${RMGR_ID_LANGUAGES}
)

include(GNUInstallDirs)

add_library(rmgr-identify INTERFACE)
target_include_directories(rmgr-identify INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")


if (RMGR_ID_BUILD_TESTS)
    add_executable(rmgr-identify-tests
        "tests/rmgr-identify-tests.cpp"
        "tests/rmgr-identify-tests.h"
        "tests/rmgr-identify-compiler.cpp"
        "tests/rmgr-identify-c-runtime.c"
        "tests/rmgr-identify-cpp-runtime.cpp"
        "include/rmgr/identify-compiler.h"
        "include/rmgr/identify-runtime.h"
    )
    target_link_libraries(rmgr-identify-tests PRIVATE rmgr-identify)
    set_property(DIRECTORY . PROPERTY VS_STARTUP_PROJECT "rmgr-identify-tests")
    if (CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
        target_compile_options(rmgr-identify-tests PRIVATE
            "-Wall"
            "-Wextra"
            "-Wundef"
        )
    elseif (MSVC)
        target_compile_options(rmgr-identify-tests PRIVATE
            "/W4"
            "/w44668" # 'symbol' is not defined as a preprocessor macro, replacing with '0' for 'directives'
        )
    endif()

    # Identify compiler
    if (MINGW)
        target_link_options(rmgr-identify-tests PRIVATE "-static")
        set(RMGR_ID_TESTS_EXPECTED_VARIANT "\"MinGW\"")
    endif()
    if (CMAKE_ISPC_COMPILER)
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND         "\"ISPC\"")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_ISPC_COMPILER_VERSION}\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND          "\"LLVM\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION  __clang_version__)
        set_source_files_properties(
            "tests/rmgr-identify-compiler.cpp"
            PROPERTIES LANGUAGE ISPC
        )
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND         "\"MSVC\"")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND          "\"MSVC\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set_property(SOURCE "tests/rmgr-identify-tests.cpp" APPEND PROPERTY COMPILE_DEFINITIONS _CRT_SECURE_NO_WARNINGS)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND         "\"GCC\"")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND          "\"GCC\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND CMAKE_GENERATOR_TOOLSET MATCHES "_clang_c2$")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND         "\"Clang\"")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND          "\"MSVC\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION  "\"19.0.0\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND         "\"Clang\"")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND          "\"LLVM\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        include(CheckCXXSymbolExists)
        check_cxx_symbol_exists("__aocc__" "" __aocc__)
        if (__aocc__)
            set(RMGR_ID_TESTS_EXPECTED_VARIANT               "\"AOCC\"")
            set(RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION_MAJOR "__aocc_major__")
            set(RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION_MINOR "__aocc_minor__")
            set(RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION_PATCH "__aocc_patchlevel__")
        endif()
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND         "\"EDG\"")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION "\"6\"") # TODO: retrieve actual version
        set(RMGR_ID_TESTS_EXPECTED_BACKEND          "\"ICC\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND         "\"Clang\"")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION __clang_version__)
        set(RMGR_ID_TESTS_EXPECTED_BACKEND          "\"LLVM\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION  __clang_version__)
        set(RMGR_ID_TESTS_EXPECTED_VARIANT          "\"ICX\"")
        set(RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Borland")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND         "\"Borland\"")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION "\"${CMAKE_CXX_COMPILER_VERSION}\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND          "\"Borland\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom")
        string(REGEX MATCH "^[0-9]+" RMGR_ID_WATCOM_VERSION_MAJOR "${CMAKE_CXX_COMPILER_VERSION}")
        math(EXPR RMGR_ID_WATCOM_VERSION_MAJOR "${RMGR_ID_WATCOM_VERSION_MAJOR}+11")
        string (REGEX REPLACE "^[0-9]+" "${RMGR_ID_WATCOM_VERSION_MAJOR}" RMGR_ID_WATCOM_VERSION "${CMAKE_CXX_COMPILER_VERSION}")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND         "\"Watcom\"")
        set(RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION "\"${RMGR_ID_WATCOM_VERSION}\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND          "\"Watcom\"")
        set(RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION  "\"${RMGR_ID_WATCOM_VERSION}\"")
        set(RMGR_ID_TESTS_EXPECTED_VARIANT          "\"Open Watcom\"")
        set(RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION  "\"${CMAKE_CXX_COMPILER_VERSION}\"")
    else()
        message(FATAL_ERROR "Unsupported/unrecognized compiler")
    endif()

    # Identify C runtime library
    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(RMGR_ID_TESTS_EXPECTED_CRT "\"glibc\"")
    elseif (CMAKE_C_COMPILER_ID STREQUAL "OpenWatcom")
        set(RMGR_ID_TESTS_EXPECTED_CRT "\"Watcom\"")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(RMGR_ID_TESTS_EXPECTED_CRT "\"MSVCRT\"")
    elseif (CYGWIN)
        set(RMGR_ID_TESTS_EXPECTED_CRT "\"Newlib\"")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
        set(RMGR_ID_TESTS_EXPECTED_CRT "\"Bionic\"")
    else()
        message(FATAL_ERROR "Unsupported/unrecognized C library")
    endif()

    # Identify C++ runtime library
    if (CMAKE_CXX_FLAGS MATCHES "stdlib=libc\\+\\+")
        set(RMGR_ID_TESTS_EXPECTED_CPPRT "\"libc++\"")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Android")
        if (ANDROID_STL MATCHES "^c\\+\\+")
            set(RMGR_ID_TESTS_EXPECTED_CPPRT "\"libc++\"")
        elseif (ANDROID_STL MATCHES "^gnustl")
            set(RMGR_ID_TESTS_EXPECTED_CPPRT "\"libstdc++\"")
        elseif (ANDROID_STL MATCHES "^stlport")
            set(RMGR_ID_TESTS_EXPECTED_CPPRT "\"STLport\"")
        elseif (ANDROID_STL STREQUAL "system" OR ANDROID_STL STREQUAL "none")
            set(RMGR_ID_TESTS_EXPECTED_CPPRT "\"\"")
        else()
            message(FATAL_ERROR "Cannot determine Android's STL")
        endif()
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CYGWIN OR MINGW)
        set(RMGR_ID_TESTS_EXPECTED_CPPRT "\"libstdc++\"")
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "OpenWatcom")
        set(RMGR_ID_TESTS_EXPECTED_CPPRT "\"Watcom\"")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(RMGR_ID_TESTS_EXPECTED_CPPRT "\"MS STL\"")
    else()
        message(FATAL_ERROR "Unsupported/unrecognized C++ library")
    endif()

    # Forward info to test program
    set_property(SOURCE "tests/rmgr-identify-tests.cpp" APPEND PROPERTY COMPILE_DEFINITIONS
       "EXPECTED_FRONTEND=${RMGR_ID_TESTS_EXPECTED_FRONTEND}"
       "EXPECTED_FRONTEND_VERSION=${RMGR_ID_TESTS_EXPECTED_FRONTEND_VERSION}"
       "EXPECTED_BACKEND=${RMGR_ID_TESTS_EXPECTED_BACKEND}"
       "EXPECTED_BACKEND_VERSION=${RMGR_ID_TESTS_EXPECTED_BACKEND_VERSION}"
       "EXPECTED_CRT=${RMGR_ID_TESTS_EXPECTED_CRT}"
       "EXPECTED_CPPRT=${RMGR_ID_TESTS_EXPECTED_CPPRT}"
    )
    if (DEFINED RMGR_ID_TESTS_EXPECTED_VARIANT)
        set_property(SOURCE "tests/rmgr-identify-tests.cpp" APPEND PROPERTY COMPILE_DEFINITIONS "EXPECTED_VARIANT=${RMGR_ID_TESTS_EXPECTED_VARIANT}")
    endif()
    if (DEFINED RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION)
        set_property(SOURCE "tests/rmgr-identify-tests.cpp" APPEND PROPERTY COMPILE_DEFINITIONS "EXPECTED_VARIANT_VERSION=${RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION}")
    elseif (DEFINED RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION_MAJOR)
        set_property(SOURCE "tests/rmgr-identify-tests.cpp" APPEND PROPERTY COMPILE_DEFINITIONS
            "EXPECTED_VARIANT_VERSION_MAJOR=${RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION_MAJOR}"
            "EXPECTED_VARIANT_VERSION_MINOR=${RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION_MINOR}"
            "EXPECTED_VARIANT_VERSION_PATCH=${RMGR_ID_TESTS_EXPECTED_VARIANT_VERSION_PATCH}"
        )
    endif()

    include(CTest)
    add_test(
        NAME    "rmgr-identify-tests"
        COMMAND "rmgr-identify-tests"
    )
endif()
