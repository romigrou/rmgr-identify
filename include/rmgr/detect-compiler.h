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

#ifndef RMGR_DETECT_COMPILER_H
#define RMGR_DETECT_COMPILER_H


/** @cond 7bad46e9372d9f26b69b05cab5a770c4 */
#define INTERNAL_RMGR_COMPILER_IS_NOT_DOXYGEN
/** @endcond */


/* ========================================================================= */
/* Detect compiler front-end                                                 */

#ifndef INTERNAL_RMGR_COMPILER_IS_NOT_DOXYGEN
    /**
     * @def   RMGR_COMPILER_FRONTEND_IS_DOXYGEN
     * @brief Whether the compiler front-end is Doxygen
     * @warning Version is always defined as 1.0.0
     */
    #define RMGR_COMPILER_FRONTEND_IS_DOXYGEN     (1)
    #define RMGR_COMPILER_FRONTEND_VERSION_MAJOR  (1)
    #define RMGR_COMPILER_FRONTEND_VERSION_MINOR  (0)
    #define RMGR_COMPILER_FRONTEND_VERSION_PATCH  (0)
    #define RMGR_COMPILER_FRONTEND_VERSION_PATCH  (0)

#elif defined(__EDG__) && (!defined(__INTELLISENSE__) || defined(__INTEL_COMPILER))
    /* Note: Intel ICC uses the EDG frontend, just as Intellisense does, hence the above conditon.
     *       However, the EDG version used for syntax higlighting will be that of Intellisense,
     *       which is not perfect but should be good enough */

    /**
     * @def   RMGR_COMPILER_FRONTEND_IS_EDG
     * @brief Whether the compiler front-end is Edison Design Group's
     * @note  Despite its using the EDG front-end, Intellisense is not reported as such.
     *        This is in order for syntax highlighting to match as closely as possible what the actual compiler does.
     */
    #define RMGR_COMPILER_FRONTEND_IS_EDG         (1)
    #define RMGR_COMPILER_FRONTEND_VERSION_MAJOR  (__EDG_VERSION__ / 100)
    #define RMGR_COMPILER_FRONTEND_VERSION_MINOR  (__EDG_VERSION__ % 100)
    #define RMGR_COMPILER_FRONTEND_VERSION_PATCH  (0)

#elif defined(__clang__)
    /**
     * @def   RMGR_COMPILER_FRONTEND_IS_CLANG
     * @brief Whether the compiler front-end is Clang
     */
    #define RMGR_COMPILER_FRONTEND_IS_CLANG       (1)
    #ifdef __INTELLISENSE__
        /* Clang version number is not reliable in Intellisense, let's do our own (rough) mapping */
        #if _MSC_VER >= 1930   /* VS2022 and above */
            #define RMGR_COMPILER_FRONTEND_VERSION_MAJOR  16
            #define RMGR_COMPILER_FRONTEND_VERSION_MINOR  0
        #elif _MSC_VER >= 1920 /* VS 2019 */
            #define RMGR_COMPILER_FRONTEND_VERSION_MAJOR  12
            #define RMGR_COMPILER_FRONTEND_VERSION_MINOR  0
        #else                  /* VS2017 and earlier: assume we're dealing with Clang/C2 */
            #define RMGR_COMPILER_FRONTEND_VERSION_MAJOR  3
            #define RMGR_COMPILER_FRONTEND_VERSION_MINOR  8
        #endif
    #else
        #define RMGR_COMPILER_FRONTEND_VERSION_MAJOR  (__clang_major__)
        #define RMGR_COMPILER_FRONTEND_VERSION_MINOR  (__clang_minor__)
        #define RMGR_COMPILER_FRONTEND_VERSION_PATCH  (__clang_patchlevel__)
    #endif

#elif defined(_MSC_VER)
    /**
     * @def   RMGR_COMPILER_FRONTEND_IS_MSVC
     * @brief Whether the compiler front-end is Microsoft Visual C++
     */
    #define RMGR_COMPILER_FRONTEND_IS_MSVC        (1)
    #define RMGR_COMPILER_FRONTEND_VERSION_MAJOR  (_MSC_VER / 100)
    #define RMGR_COMPILER_FRONTEND_VERSION_MINOR  (_MSC_VER % 100)
#ifdef _MSC_FULL_VER
    #define RMGR_COMPILER_FRONTEND_VERSION_PATCH  (_MSC_FULL_VER % 100000)
#endif

#elif defined(__GNUC__)
    /**
     * @def   RMGR_COMPILER_FRONTEND_IS_GCC
     * @brief Whether the compiler front-end is GCC
     */
    #define RMGR_COMPILER_FRONTEND_IS_GCC         (1)
    #define RMGR_COMPILER_FRONTEND_VERSION_MAJOR  (__GNUC__)
    #define RMGR_COMPILER_FRONTEND_VERSION_MINOR  (__GNUC_MINOR__)
#ifdef __GNUC_PATCHLEVEL__
    #define RMGR_COMPILER_FRONTEND_VERSION_PATCH  (__GNUC_PATCHLEVEL__)
#endif

#elif defined(RMGR_DETECT_NO_FAILURE)
    #define RMGR_COMPILER_FRONTEND_IS_UNKNOWN     (1)
#else
    #error Unsupported/unrecognized compiler front-end
#endif


/* Set default values to undefined macros */
#ifndef RMGR_COMPILER_FRONTEND_IS_CLANG
    #define RMGR_COMPILER_FRONTEND_IS_CLANG       (0)
#endif
#ifndef RMGR_COMPILER_FRONTEND_IS_DOXYGEN
    #define RMGR_COMPILER_FRONTEND_IS_DOXYGEN     (0)
#endif
#ifndef RMGR_COMPILER_FRONTEND_IS_EDG
    #define RMGR_COMPILER_FRONTEND_IS_EDG         (0)
#endif
#ifndef RMGR_COMPILER_FRONTEND_IS_GCC
    #define RMGR_COMPILER_FRONTEND_IS_GCC         (0)
#endif
#ifndef RMGR_COMPILER_FRONTEND_IS_MSVC
    #define RMGR_COMPILER_FRONTEND_IS_MSVC        (0)
#endif
#ifndef RMGR_COMPILER_FRONTEND_IS_UNKNOWN
    #define RMGR_COMPILER_FRONTEND_IS_UNKNOWN     (0)
#endif
#ifndef RMGR_COMPILER_FRONTEND_VERSION_PATCH
    #define RMGR_COMPILER_FRONTEND_VERSION_PATCH  (0)
#endif


/* ========================================================================= */
/* Detect compiler back-end                                                  */

#if    defined(__llvm__) \
    || (defined(__INTELLISENSE__) && defined(__clang__) && _MSC_VER >= 1920) /* Workaround for Intellisense not definining __llvm__ */
    /**
     * @def   RMGR_COMPILER_BACKEND_IS_LLVM
     * @brief Whether the compiler back-end is LLVM
     */
    #define RMGR_COMPILER_BACKEND_IS_LLVM        (1)
    #if RMGR_COMPILER_FRONTEND_IS_CLANG
        #define RMGR_COMPILER_BACKEND_VERSION_MAJOR  RMGR_COMPILER_FRONTEND_VERSION_MAJOR
        #define RMGR_COMPILER_BACKEND_VERSION_MINOR  RMGR_COMPILER_FRONTEND_VERSION_MINOR
        #define RMGR_COMPILER_BACKEND_VERSION_PATCH  RMGR_COMPILER_FRONTEND_VERSION_PATCH
    #else
        #error Unable to determine LLVM version
    #endif

#elif defined(__INTEL_COMPILER)
    /**
     * @def   RMGR_COMPILER_BACKEND_IS_ICC
     * @brief Whether the compiler back-end is Intel C++ Compiler Classic
     */
    #define RMGR_COMPILER_BACKEND_IS_ICC         (1)
    #if __INTEL_COMPILER >= 2000
        #define RMGR_COMPILER_BACKEND_VERSION_MAJOR  (__INTEL_COMPILER)
        #ifdef __INTEL_COMPILER_UPDATE
        #define RMGR_COMPILER_BACKEND_VERSION_MINOR  (__INTEL_COMPILER_UPDATE)
        #endif
    #else
        #define RMGR_COMPILER_BACKEND_VERSION_MAJOR  (__INTEL_COMPILER / 100)
        #define RMGR_COMPILER_BACKEND_VERSION_MINOR  (__INTEL_COMPILER % 100)
        #ifdef __INTEL_COMPILER_UPDATE
        #define RMGR_COMPILER_BACKEND_VERSION_PATCH  (__INTEL_COMPILER_UPDATE)
        #endif
    #endif

#elif defined(_MSC_VER)
    /**
     * @def   RMGR_COMPILER_FRONTEND_IS_MSVC
     * @brief Whether the compiler front-end is Microsoft Visual C++
     */
    #define RMGR_COMPILER_BACKEND_IS_MSVC        (1)
    #define RMGR_COMPILER_BACKEND_VERSION_MAJOR  (_MSC_VER / 100)
    #define RMGR_COMPILER_BACKEND_VERSION_MINOR  (_MSC_VER % 100)
#ifdef _MSC_FULL_VER
    #define RMGR_COMPILER_BACKEND_VERSION_PATCH  (_MSC_FULL_VER % 100000)
#endif

#elif defined(__GNUC__)
    /**
     * @def   RMGR_COMPILER_FRONTEND_IS_GCC
     * @brief Whether the compiler front-end is GCC
     */
    #define RMGR_COMPILER_BACKEND_IS_GCC          (1)
    #define RMGR_COMPILER_BACKEND_VERSION_MAJOR   (__GNUC__)
    #define RMGR_COMPILER_BACKEND_VERSION_MINOR   (__GNUC_MINOR__)
#ifdef __GNUC_PATCHLEVEL__
    #define RMGR_COMPILER_BACKEND_VERSION_PATCH   (__GNUC_PATCHLEVEL__)
#endif

#elif defined(RMGR_DETECT_NO_FAILURE)
    #define RMGR_COMPILER_BACKEND_IS_UNKNOWN      (1)
#else
    #error Unsupported/unrecognized compiler back-end
#endif


/* Set default values to undefined macros */
#ifndef RMGR_COMPILER_BACKEND_IS_GCC
    #define RMGR_COMPILER_BACKEND_IS_GCC         (0)
#endif
#ifndef RMGR_COMPILER_BACKEND_IS_ICC
    #define RMGR_COMPILER_BACKEND_IS_ICC         (0)
#endif
#ifndef RMGR_COMPILER_BACKEND_IS_LLVM
    #define RMGR_COMPILER_BACKEND_IS_LLVM        (0)
#endif
#ifndef RMGR_COMPILER_BACKEND_IS_MSVC
    #define RMGR_COMPILER_BACKEND_IS_MSVC        (0)
#endif
#ifndef RMGR_COMPILER_BACKEND_VERSION_MINOR
    #define RMGR_COMPILER_BACKEND_VERSION_MINOR  (0)
#endif
#ifndef RMGR_COMPILER_BACKEND_VERSION_PATCH
    #define RMGR_COMPILER_BACKEND_VERSION_PATCH  (0)
#endif


/* ========================================================================= */
/* Detect compiler variant                                                   */

#ifdef __MINGW32__
    #define RMGR_COMPILER_VARIANT_IS_MINGW  (1)
#elif RMGR_COMPILER_FRONTEND_IS_CLANG && RMGR_COMPILER_BACKEND_IS_LLVM
    #if defined(__aocc__)
        /**
         * @def   RMGR_COMPILER_VARIANT_IS_AOCC
         * @brief Whether the compiler variant is AMD Optimizing C/C++
         */
        #define RMGR_COMPILER_VARIANT_IS_AOCC        (1)
        #define RMGR_COMPILER_VARIANT_VERSION_MAJOR  (__aocc_major__)
        #define RMGR_COMPILER_VARIANT_VERSION_MINOR  (__aocc_minor__)
        #define RMGR_COMPILER_VARIANT_VERSION_PATCH  (__aocc_patchlevel__)
    #elif defined(__INTEL_CLANG_COMPILER)
        /**
         * @def   RMGR_COMPILER_VARIANT_IS_ICX
         * @brief Whether the compiler variant is Intel oneAPI DPC++/C++
         */
        #define RMGR_COMPILER_VARIANT_IS_ICX         (1)
        #define RMGR_COMPILER_VARIANT_VERSION_MAJOR  (__INTEL_CLANG_COMPILER / 10000)
        #define RMGR_COMPILER_VARIANT_VERSION_MINOR  ((__INTEL_CLANG_COMPILER / 100) % 100)
        #define RMGR_COMPILER_VARIANT_VERSION_PATCH  (__INTEL_CLANG_COMPILER % 100)
    #else
        #define RMGR_COMPILER_VARIANT_IS_UNKNOWN     (1)
    #endif
#else
    /**
     * @def   RMGR_COMPILER_VARIANT_IS_UNKNOWN
     * @brief If non-zero, the compiler variant is unknown (or is not a variant at all)
     */
    #define RMGR_COMPILER_VARIANT_IS_UNKNOWN     (1)
#endif


/* Set default values to undefined macros */
#ifndef RMGR_COMPILER_VARIANT_IS_AOCC
    #define RMGR_COMPILER_VARIANT_IS_AOCC   (0)
#endif
#ifndef RMGR_COMPILER_VARIANT_IS_ICX
    #define RMGR_COMPILER_VARIANT_IS_ICX    (0)
#endif
#ifndef RMGR_COMPILER_VARIANT_IS_MINGW
    #define RMGR_COMPILER_VARIANT_IS_MINGW  (0)
#endif


/* ========================================================================= */
/* Utility macros                                                            */

#ifndef INTERNAL_RMGR_VERSION_COMP_LT
    #define INTERNAL_RMGR_VERSION_COMP_LT(a1,a2,a3,b1,b2,b3)            ((a1) < (b1) || ((a1) == (b1) && ((a2) < (b2) || ((a2) == (b2) && (a3) < (b3)))))
#endif

#ifndef RMGR_VERSION_IS_LESS_THAN
    /**
     * @brief Evaluates to non-zero if the version number is < the specified one
     */
    #define RMGR_VERSION_IS_LESS_THAN(name,major,minor,patch)           INTERNAL_RMGR_VERSION_COMP_LT(name##_MAJOR, name##_MINOR, name##_PATCH, major, minor, patch)
#endif

#ifndef RMGR_VERSION_IS_GREATER_THAN
    /**
     * @brief Evaluates to non-zero if the version number is >= the specified one
     */
    #define RMGR_VERSION_IS_GREATER_THAN(name,major,minor,patch)        !(INTERNAL_RMGR_VERSION_COMP_LT(name##_MAJOR, name##_MINOR, name##_PATCH, major, minor, patch))
#endif

/**
 * @brief Evaluates to non-zero if the compiler front-end's version is < the specified one
 */
#define RMGR_COMPILER_FRONTEND_VERSION_IS_LESS_THAN(major,minor,patch)  RMGR_VERSION_IS_LESS_THAN(RMGR_COMPILER_FRONTEND_VERSION, major, minor, patch)

/**
 * @brief Evaluates to non-zero if the compiler front-end's version is >= the specified one
 */
#define RMGR_COMPILER_FRONTEND_VERSION_IS_AT_LEAST(major,minor,patch)   RMGR_VERSION_IS_GREATER_THAN(RMGR_COMPILER_FRONTEND_VERSION, major, minor, patch)

/**
 * @brief Evaluates to non-zero if the compiler back-end's version is < the specified one
 */
#define RMGR_COMPILER_BACKEND_VERSION_IS_LESS_THAN(major,minor,patch)   RMGR_VERSION_IS_LESS_THAN(RMGR_COMPILER_BACKEND_VERSION, major, minor, patch)

/**
 * @brief Evaluates to non-zero if the compiler back-end's version is >= the specified one
 */
#define RMGR_COMPILER_BACKEND_VERSION_IS_AT_LEAST(major,minor,patch)    RMGR_VERSION_IS_GREATER_THAN(RMGR_COMPILER_BACKEND_VERSION, major, minor, patch)

/**
 * @brief Convenience macros for testing both front-end and back-end
 */
#define RMGR_COMPILER_IS(frontend, backend)  (RMGR_COMPILER_FRONTEND_IS_##frontend && RMGR_COMPILER_BACKEND_IS_##backend)


/* ========================================================================= */
/* Convenience all-in-one compiler identification macros                     */

/* Front-end + back-end */
#define RMGR_COMPILER_IS_CLANG_LLVM  RMGR_COMPILER_IS(CLANG, LLVM)
#define RMGR_COMPILER_IS_GCC         RMGR_COMPILER_IS(GCC,   GCC)
#define RMGR_COMPILER_IS_ICC         RMGR_COMPILER_IS(EDG,   ICC)
#define RMGR_COMPILER_IS_MSVC        RMGR_COMPILER_IS(MSVC,  MSVC)


/* Front-end + back-end + version */
#define RMGR_COMPILER_IS_CLANG_LLVM_AT_LEAST(major,minor,patch)  (RMGR_COMPILER_FRONTEND_VERSION_IS_AT_LEAST((major),(minor),(patch)) && RMGR_COMPILER_IS_CLANG_LLVM)
#define RMGR_COMPILER_IS_GCC_AT_LEAST(major,minor,patch)         (RMGR_COMPILER_FRONTEND_VERSION_IS_AT_LEAST((major),(minor),(patch)) && RMGR_COMPILER_IS_GCC)
#define RMGR_COMPILER_IS_ICC_AT_LEAST(major,minor,patch)         (RMGR_COMPILER_FRONTEND_VERSION_IS_AT_LEAST((major),(minor),(patch)) && RMGR_COMPILER_IS_ICC)
#define RMGR_COMPILER_IS_MSVC_AT_LEAST(major,minor,patch)        (RMGR_COMPILER_FRONTEND_VERSION_IS_AT_LEAST((major),(minor),(patch)) && RMGR_COMPILER_IS_MSVC)


#endif /* RMGR_DETECT_COMPILER_H */
