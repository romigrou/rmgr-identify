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

#include <rmgr/detect-compiler.h>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>


static bool check_version_numbers(const char* v1, const char* v2)
{
    while (*v1 && *v1==*v2)
    {
        ++v1;
        ++v2;
    }

    // It's OK for numbers to differ iff one is more detailed than the other one
    return (!*v1 && !isdigit(*v2)) || (!*v2 && !isdigit(*v1));
}


#ifndef EXPECTED_VARIANT_VERSION
    #ifdef EXPECTED_VARIANT_VERSION_MAJOR
        #define STRINGIFY(a)              DO_STRINGIFY(a)
        #define DO_STRINGIFY(a)           #a
        #define EXPECTED_VARIANT_VERSION  STRINGIFY(EXPECTED_VARIANT_VERSION_MAJOR) "." STRINGIFY(EXPECTED_VARIANT_VERSION_MINOR) "." STRINGIFY(EXPECTED_VARIANT_VERSION_PATCH)
    #else
        #define EXPECTED_VARIANT_VERSION  ""
    #endif
#endif


int main()
{
#if RMGR_COMPILER_FRONTEND_IS_CLANG
    static const char frontEndName[] = {"Clang"};
#endif
#if RMGR_COMPILER_FRONTEND_IS_EDG
    static const char frontEndName[] = {"EDG"};
#endif
#if RMGR_COMPILER_FRONTEND_IS_GCC
    static const char frontEndName[] = {"GCC"};
#endif
#if RMGR_COMPILER_FRONTEND_IS_MSVC
    static const char frontEndName[] = {"MSVC"};
#endif


#if RMGR_COMPILER_BACKEND_IS_GCC
    static const char backEndName[] = {"GCC"};
#endif
#if RMGR_COMPILER_BACKEND_IS_ICC
    static const char backEndName[] = {"ICC"};
#endif
#if RMGR_COMPILER_BACKEND_IS_LLVM
    static const char backEndName[] = {"LLVM"};
#endif
#if RMGR_COMPILER_BACKEND_IS_MSVC
    static const char backEndName[] = {"MSVC"};
#endif


#if RMGR_COMPILER_VARIANT_IS_AOCC
    static const char variantName[] = {"AOCC"};
#endif
#if RMGR_COMPILER_VARIANT_IS_ICX
    static const char variantName[] = {"ICX"};
#endif
#if RMGR_COMPILER_VARIANT_IS_MINGW
    static const char variantName[] = {"MinGW"};
#endif


    char frontEndVersion[64];
    snprintf(frontEndVersion, sizeof(frontEndVersion), "%u.%u.%u", RMGR_COMPILER_FRONTEND_VERSION_MAJOR, RMGR_COMPILER_FRONTEND_VERSION_MINOR, RMGR_COMPILER_FRONTEND_VERSION_PATCH);

    char backEndVersion[64];
    snprintf(backEndVersion, sizeof(backEndVersion), "%u.%u.%u", RMGR_COMPILER_BACKEND_VERSION_MAJOR, RMGR_COMPILER_BACKEND_VERSION_MINOR, RMGR_COMPILER_BACKEND_VERSION_PATCH);

#ifdef RMGR_COMPILER_VARIANT_VERSION_MAJOR
    char variantVersion[64];
    snprintf(variantVersion, sizeof(variantVersion), "%u.%u.%u", RMGR_COMPILER_VARIANT_VERSION_MAJOR, RMGR_COMPILER_VARIANT_VERSION_MINOR, RMGR_COMPILER_VARIANT_VERSION_PATCH);
#else
    static const char variantVersion[] = {""};
#endif

    printf("Detected front-end: %s %s\n",   frontEndName, frontEndVersion);
    printf("Expected front-end: %s %s\n\n", EXPECTED_FRONTEND, EXPECTED_FRONTEND_VERSION);
    printf("Detected back-end:  %s %s\n",   backEndName, backEndVersion);
    printf("Expected back-end:  %s %s\n\n", EXPECTED_BACKEND, EXPECTED_BACKEND_VERSION);
#ifdef EXPECTED_VARIANT
    printf("Detected variant:   %s %s\n",   variantName, variantVersion);
    printf("Expected variant:   %s %s\n\n", EXPECTED_VARIANT, EXPECTED_VARIANT_VERSION);
#endif

    if (strcmp(frontEndName, EXPECTED_FRONTEND) != 0)
    {
        fprintf(stderr, "Front-end mismatch\n");
        return EXIT_FAILURE;
    }
    if (!check_version_numbers(frontEndVersion, EXPECTED_FRONTEND_VERSION))
    {
        fprintf(stderr, "Front-end version mismatch\n");
        return EXIT_FAILURE;
    }

    if (strcmp(backEndName, EXPECTED_BACKEND) != 0)
    {
        fprintf(stderr, "Back-end mismatch\n");
        return EXIT_FAILURE;
    }
    if (!check_version_numbers(backEndVersion, EXPECTED_BACKEND_VERSION))
    {
        fprintf(stderr, "Back-end version mismatch\n");
        return EXIT_FAILURE;
    }

#ifdef EXPECTED_VARIANT
    if (strcmp(variantName, EXPECTED_VARIANT) != 0)
    {
        fprintf(stderr, "Variant mismatch\n");
        return EXIT_FAILURE;
    }
    if (strcmp(variantVersion, EXPECTED_VARIANT_VERSION) != 0)
    {
        fprintf(stderr, "Variant version mismatch\n");
        return EXIT_FAILURE;
    }
#endif

    printf("Success!\n");
    return EXIT_SUCCESS;
}
