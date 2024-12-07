/*
 * This software is available under 2 licenses -- choose whichever you prefer.
 *
 * -------------------------------------------------------------------------------
 *
 * Copyright (c) 2023 Romain BAILLY
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * -------------------------------------------------------------------------------
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org/>
 */


#ifndef RMGR_ID_RUNTIME_H
#define RMGR_ID_RUNTIME_H

/* The minimal header that allows us to identify the C library seems to be string.h.
   However, if we can include a more pinpointed header, let's do it. */
#ifdef __has_include
    #if __has_include(<_newlib_version.h>)
        #include <_newlib_version.h>
    #elif __has_include(<features.h>)
        #include <features.h>
    #else
        #define INTERNAL_RMGR_CRT_NO_PINPOINTED_HEADER
    #endif
#else
    #define INTERNAL_RMGR_CRT_NO_PINPOINTED_HEADER
#endif

#ifdef INTERNAL_RMGR_CRT_NO_PINPOINTED_HEADER
    #undef INTERNAL_RMGR_CRT_NO_PINPOINTED_HEADER
    #ifndef _WIN32 /* No need to include anything on Windows */
        #ifdef __cplusplus
            #include <cstring>
        #else
            #include "string.h"
        #endif
    #endif
#endif

/* ========================================================================= */
/* Identify the C library                                                    */

#if defined(__GNU_LIBRARY__)
    /**
     * @def   RMGR_CRT_IS_GLIBC
     * @brief Whether the C library is glibc
     */
    #define RMGR_CRT_IS_GLIBC       (1)
    #if defined(__GLIBC__)
        #define RMGR_CRT_VERSION_MAJOR  (__GLIBC__)
        #define RMGR_CRT_VERSION_MINOR  (__GLIBC_MINOR__)
    #else
        #define RMGR_CRT_VERSION_MAJOR  (__GNU_LIBRARY__)
        #define RMGR_CRT_VERSION_MINOR  (__GNU_LIBRARY_MINOR__)
    #endif

#elif defined(__NEWLIB__)
    /**
     * @def   RMGR_CRT_IS_NEWLIB
     * @brief Whether the C library is Newlilb
     */
    #define RMGR_CRT_IS_NEWLIB      (1)
    #define RMGR_CRT_VERSION_MAJOR  (__NEWLIB__)
    #define RMGR_CRT_VERSION_MINOR  (__NEWLIB_MINOR__)
    #define RMGR_CRT_VERSION_PATCH  (__NEWLIB_PATCHLEVEL__)

#elif defined(__BIONIC__)
    /**
     * @def   RMGR_CRT_IS_BIONIC
     * @brief Whether the C library is Bionic
     *
     * @note As Bionic seems to have no version number the Android API level is used as the minor version number
     *       (so as to maximize chances to maintain monotonicity if a genuine version number appears some day).
     */
    #define RMGR_CRT_IS_BIONIC      (1)
    #define RMGR_CRT_VERSION_MAJOR  (0)
    #define RMGR_CRT_VERSION_MINOR  (__ANDROID_API__)
    #define RMGR_CRT_VERSION_PATCH  (0)

#elif defined(_WIN32)
    #ifdef __BORLANDC__
        #define RMGR_CRT_IS_BORLAND     (1)
        #define RMGR_CRT_VERSION_MAJOR  (__BORLANDC__ >> 8)
        #define RMGR_CRT_VERSION_MINOR  ((__BORLANDC__ >> 4) & 0xF)
        #define RMGR_CRT_VERSION_PATCH  (__BORLANDC__ & 0xF)
    #elif defined(__MINGW32__)
        #include <_mingw.h>
        #define RMGR_CRT_IS_MSVCRT  (1)
        #ifdef MSVCRT_VERSION
            #define RMGR_CRT_VERSION_MAJOR  (MSVCRT_VERSION / 100)
            #define RMGR_CRT_VERSION_MINOR  (MSVCRT_VERSION % 100)
        #elif defined(__MSVCRT_VERSION__)
            #define RMGR_CRT_VERSION_MAJOR  (__MSVCRT_VERSION__ / 256)
            #define RMGR_CRT_VERSION_MINOR  (__MSVCRT_VERSION__ % 256)
        #else
            #error Cannot determine the version of MSVCRT
        #endif
    #elif defined(__has_include) && !__has_include(<vcruntime.h>)
        /* This check is just for extra safety but, AFAIK, MSVCRT is the only C library used on Windows */
        #define RMGR_CRT_IS_UNKNOWN  (1)
        #error Expected C runtime library was MSVCRT but appears not to be the case
    #elif _MSC_VER < 1600
        /* Prior to VS2010 crtversion.h is not available, compute a best-effort version number */
        #define RMGR_CRT_IS_MSVCRT      (1)
        #define RMGR_CRT_VERSION_MAJOR  ((_MSC_VER / 100) - 6)
        #define RMGR_CRT_VERSION_MINOR  ((_MSC_VER % 100) / 10)
    #else
        #include <crtversion.h>
        /**
         * @def   RMGR_CRT_IS_MSVRCT
         * @brief Whether the C library is MSVCRT
         */
        #define RMGR_CRT_IS_MSVCRT      (1)
        #define RMGR_CRT_VERSION_MAJOR  (_VC_CRT_MAJOR_VERSION)
        #define RMGR_CRT_VERSION_MINOR  (_VC_CRT_MINOR_VERSION)
        #define RMGR_CRT_VERSION_PATCH  (_VC_CRT_BUILD_VERSION)
    #endif
#else
    #define RMGR_CRT_IS_UNKNOWN     (1)
#endif

#if defined(RMGR_CRT_IS_UNKNOWN) && RMGR_CRT_IS_UNKNOWN && !defined(RMGR_ID_NO_FAILURE)
    #error Unsupported/unrecognized C library
#endif


/* Set default values to undefined macros */
#ifndef RMGR_CRT_IS_BIONIC
    #define RMGR_CRT_IS_BIONIC      (0)
#endif
#ifndef RMGR_CRT_IS_BORLAND
    #define RMGR_CRT_IS_BORLAND     (0)
#endif
#ifndef RMGR_CRT_IS_GLIBC
    #define RMGR_CRT_IS_GLIBC       (0)
#endif
#ifndef RMGR_CRT_IS_MSVCRT
    #define RMGR_CRT_IS_MSVCRT      (0)
#endif
#ifndef RMGR_CRT_IS_NEWLIB
    #define RMGR_CRT_IS_NEWLIB      (0)
#endif
#ifndef RMGR_CRT_IS_UNKNOWN
    #define RMGR_CRT_IS_UNKNOWN     (0)
#endif
#ifndef RMGR_CRT_VERSION_PATCH
    #define RMGR_CRT_VERSION_PATCH  (0)
#endif


/* ========================================================================= */
/* Identify the C++ library                                                  */

#ifdef __cplusplus

#ifdef __has_include
    #if __has_include(<version>)
        #include <version>
    #elif __has_include(<bits/c++config.h>)
        #include <bits/c++config.h>
    #elif __has_include(<__config>)
        #include <__config>
    #elif __has_include(<_stlport_version.h>)
        #include <_stlport_version.h>
    #elif __has_include(<ciso646>)
        #include <ciso646>
    #else
        #define INTERNAL_RMGR_CRT_NO_PINPOINTED_HEADER
    #endif
#else
    #define INTERNAL_RMGR_CRT_NO_PINPOINTED_HEADER
#endif
#ifdef INTERNAL_RMGR_CRT_NO_PINPOINTED_HEADER
    #undef INTERNAL_RMGR_CRT_NO_PINPOINTED_HEADER
    #include <new>
#endif

#if defined(__GLIBCXX__) || defined(__GLIBCPP__)
    /**
     * @def   RMGR_CPPRT_IS_LIBSTDCPP
     * @brief Whether the C++ library is GNU's libstdc++
     */
    #define RMGR_CPPRT_IS_LIBSTDCPP   (1)
    #ifdef __GLIBCXX__
        #define RMGR_CPPRT_VERSION_MAJOR  (__GLIBCXX__ / 10000)
        #define RMGR_CPPRT_VERSION_MINOR  ((__GLIBCXX__ / 100) % 100)
        #define RMGR_CPPRT_VERSION_PATCH  (__GLIBCXX__ % 100)
    #else
        #define RMGR_CPPRT_VERSION_MAJOR  (__GLIBCPP__ / 10000)
        #define RMGR_CPPRT_VERSION_MINOR  ((__GLIBCPP__ / 100) % 100)
        #define RMGR_CPPRT_VERSION_PATCH  (__GLIBCPP__ % 100)
    #endif

#elif defined(_LIBCPP_VERSION)
    /**
     * @def   RMGR_CPPRT_IS_LIBCPP
     * @brief Whether the C++ library is LLVM's libc++
     */
    #define RMGR_CPPRT_IS_LIBCPP      (1)
    // Numbering scheme changed in version 16
    // See https://github.com/llvm/llvm-project/commit/b6ff5b470d5e4acfde59d57f18e575c0284941f4
    #if _LIBCPP_VERSION >= 160000
        #define RMGR_CPPRT_VERSION_MAJOR  (_LIBCPP_VERSION / 10000)
        #define RMGR_CPPRT_VERSION_MINOR  ((_LIBCPP_VERSION / 100) % 100)
        #define RMGR_CPPRT_VERSION_PATCH  (_LIBCPP_VERSION % 100)
    #else
        #define RMGR_CPPRT_VERSION_MAJOR  (_LIBCPP_VERSION / 1000)
        #define RMGR_CPPRT_VERSION_MINOR  ((_LIBCPP_VERSION / 100) % 10)
        #define RMGR_CPPRT_VERSION_PATCH  (_LIBCPP_VERSION % 100)
    #endif

#elif defined(_STLPORT_VERSION)
    /**
     * @def   RMGR_CPPRT_IS_STLPORT
     * @brief Whether the C++ library is STLport
     */
    #define RMGR_CPPRT_IS_STLPORT     (1)
    #define RMGR_CPPRT_VERSION_MAJOR  (_STLPORT_MAJOR)
    #define RMGR_CPPRT_VERSION_MINOR  (_STLPORT_MINOR)
    #define RMGR_CPPRT_VERSION_PATCH  (_STLPORT_PATCHLEVEL)

#elif defined(_RWSTD_VER)
    /**
     * @def   RMGR_CPPRT_IS_RWSTD
     * @brief Whether the C++ library is Rogue Wave's
     */
    #define RMGR_CPPRT_IS_RWSTD       (1)
    #define RMGR_CPPRT_VERSION_MAJOR  (_RWSTD_VER >> 24)
    #define RMGR_CPPRT_VERSION_MINOR  ((_RWSTD_VER >> 16) & 0xFF)
    #define RMGR_CPPRT_VERSION_PATCH  ((_RWSTD_VER >>_ 8) & 0xFF)

#elif defined(_MSVC_STL_UPDATE)
    /**
     * @def   RMGR_CPPRT_IS_MSSTL
     * @brief Whether the C++ library is MS STL
     *
     * @note Prior to Visual C++ 2017 15.5 (which corresponds to MSVC 19.12),
     *       MS STL identified itself as Dinkumware's STL.
     *       See https://devblogs.microsoft.com/cppblog/c17-progress-in-vs-2017-15-5-and-15-6/
     *       and https://github.com/microsoft/STL/wiki/Macro-_MSVC_STL_UPDATE
     */
    #define RMGR_CPPRT_IS_MSSTL       (1)
    #define RMGR_CPPRT_VERSION_MAJOR  (_MSVC_STL_UPDATE / 100)
    #define RMGR_CPPRT_VERSION_MINOR  (_MSVC_STL_UPDATE % 100)
    
#elif defined(_CPPLIB_VER)
    /**
     * @def   RMGR_CPPRT_IS_DINKUMWARE
     * @brief Whether the C++ library is Dinkumware's
     *
     * @note Prior to Visual C++ 2017 15.5 (which corresponds to MSVC 19.12),
     *       MS STL identified itself as Dinkumware's STL.
     *       See https://devblogs.microsoft.com/cppblog/c17-progress-in-vs-2017-15-5-and-15-6/
     *       and https://github.com/microsoft/STL/wiki/Macro-_MSVC_STL_UPDATE
     */
    #define RMGR_CPPRT_IS_DINKUMWARE  (1)
    #define RMGR_CPPRT_VERSION_MAJOR  (_CPPLIB_VER / 100)
    #define RMGR_CPPRT_VERSION_MINOR  (_CPPLIB_VER % 100)

#elif defined(RMGR_ID_NO_FAILURE)
    #define RMGR_CPPRT_IS_UNKNOWN     (1)
#else
    #error Unsupported/unrecognized C++ library
#endif


/* Set default values to undefined macros */
#ifndef RMGR_CPPRT_IS_DINKUMWARE
    #define RMGR_CPPRT_IS_DINKUMWARE  (0)
#endif
#ifndef RMGR_CPPRT_IS_LIBCPP
    #define RMGR_CPPRT_IS_LIBCPP      (0)
#endif
#ifndef RMGR_CPPRT_IS_LIBSTDCPP
    #define RMGR_CPPRT_IS_LIBSTDCPP   (0)
#endif
#ifndef RMGR_CPPRT_IS_MSSTL
    #define RMGR_CPPRT_IS_MSSTL       (0)
#endif
#ifndef RMGR_CPPRT_IS_RWSTD
    #define RMGR_CPPRT_IS_RWSTD       (0)
#endif
#ifndef RMGR_CPPRT_IS_STLPORT
    #define RMGR_CPPRT_IS_STLPORT     (0)
#endif
#ifndef RMGR_CPPRT_IS_UNKNOWN
    #define RMGR_CPPRT_IS_UNKNOWN     (0)
#endif
#ifndef RMGR_CPPRT_VERSION_PATCH
    #define RMGR_CPPRT_VERSION_PATCH  (0)
#endif

#endif /* __cplusplus */


#endif /* RMGR_ID_RUNTIME_H */
