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

#include <rmgr/identify-runtime.h>
#include "rmgr-identify-tests.h"


void rmgr_identify_cpp_runtime(const char*& name, unsigned& major, unsigned& minor, unsigned& patch)
{
#if RMGR_CPPRT_IS_DINKUMWARE
    static const char cpprtName[] = {"Dinkumware"};
#endif
#if RMGR_CPPRT_IS_LIBCPP
    static const char cpprtName[] = {"libc++"};
#endif
#if RMGR_CPPRT_IS_LIBSTDCPP
    static const char cpprtName[] = {"libstdc++"};
#endif
#if RMGR_CPPRT_IS_MSSTL
    static const char cpprtName[] = {"MS STL"};
#endif
#if RMGR_CPPRT_IS_RWSTD
    static const char cpprtName[] = {"RogueWave"};
#endif
#if RMGR_CPPRT_IS_STLPORT
    static const char cpprtName[] = {"STLport"};
#endif
#if RMGR_CPPRT_IS_WATCOM
    static const char cpprtName[] = {"Watcom"};
#endif

    name = cpprtName;
    major = RMGR_CPPRT_VERSION_MAJOR;
    minor = RMGR_CPPRT_VERSION_MINOR;
    patch = RMGR_CPPRT_VERSION_PATCH;
}
