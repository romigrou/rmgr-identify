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

#include <rmgr/identify-compiler.h>
#include "rmgr-identify-tests.h"


void rmgr_identify_compiler_frontend(const char*& name, unsigned& major, unsigned& minor, unsigned& patch)
{
#if RMGR_COMPILER_FRONTEND_IS_BORLAND
    static const char frontEndName[] = {"Borland"};
#endif
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
#if RMGR_COMPILER_FRONTEND_IS_WATCOM
    static const char frontEndName[] = {"Watcom"};
#endif

    name  = frontEndName;
    major = RMGR_COMPILER_FRONTEND_VERSION_MAJOR;
    minor = RMGR_COMPILER_FRONTEND_VERSION_MINOR;
    patch = RMGR_COMPILER_FRONTEND_VERSION_PATCH;
}


void rmgr_identify_compiler_backend(const char*& name, unsigned& major, unsigned& minor, unsigned& patch)
{
#if RMGR_COMPILER_BACKEND_IS_BORLAND
    static const char backEndName[] = {"Borland"};
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
#if RMGR_COMPILER_BACKEND_IS_WATCOM
    static const char backEndName[] = {"Watcom"};
#endif

    name  = backEndName;
    major = RMGR_COMPILER_BACKEND_VERSION_MAJOR;
    minor = RMGR_COMPILER_BACKEND_VERSION_MINOR;
    patch = RMGR_COMPILER_BACKEND_VERSION_PATCH;
}


bool rmgr_identify_compiler_variant(const char*& name, unsigned& major, unsigned& minor, unsigned& patch)
{
#if RMGR_COMPILER_VARIANT_IS_UNKNOWN
    static const char variantName[] = {"<unknown>"};
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
#if RMGR_COMPILER_VARIANT_IS_OPEN_WATCOM
    static const char variantName[] = {"Open Watcom"};
#endif

    name  = variantName;
#if !RMGR_COMPILER_VARIANT_IS_UNKNOWN
    major = RMGR_COMPILER_VARIANT_VERSION_MAJOR;
    minor = RMGR_COMPILER_VARIANT_VERSION_MINOR;
    patch = RMGR_COMPILER_VARIANT_VERSION_PATCH;
#else
    (void)major;
    (void)minor;
    (void)patch;
#endif

    return !RMGR_COMPILER_VARIANT_IS_UNKNOWN;
}
