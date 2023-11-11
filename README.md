rmgr::detect
============

A small collection of header files intended to **reliably** detect the compiler and the C library.

I have seen many projects, including very popular and high-quality ones, fail to compile when using
some uncommon tool chain because of incorrect conditional compilation clauses. Typically, using
Clang under Windows is an almost-assured failure.

This project is an attempt at preventing such failures by using finer-grained tool chain detection

Detecting The Compiler
----------------------

Of course, resources such as [cpredef](https://github.com/cpredef/predef) are invaluable as
references but they fall short of being immediately useful because reality is a bit trickier. Some
compilers impersonate other ones (GCC is a popular target of impersonation) which makes proper
compiler identification order-dependent, especially if you want Intellisense to work correctly.

Another thing to consider is front-end v. back-end distinction. We tend to consider compilers as a
whole, but they are actually made of (at least) four distinct parts: the preprocessor,
the front-end, the back-end and the driver.

### The Preprocessor

No real need to explain what this is, other than to say that it is usually tightly coupled with the
front-end. Still, it remains feasible to prepocess as a separate step and then feed the result to
the compiler. Of course, different preprocessors behaving differently it's important to be able to
tell them apart.

### The Front-End

This is the part that parses the language and turns it into a syntactic tree. Popular front-ends
are [Edison Design Group](https://www.edg.com/c)'s (EDG) and Clang (yes, [Clang is just a front-end](https://clang.llvm.org/)).
EDG is used by many commercial compilers (ARM, Intel, Cuda, ...), although some of them have now
transitioned to Clang (ARM, Intel, ...).

Things that typically depend on the front-end are predefined macros and language support: which
language extensions and features are available but also how well the latter are implemented (tricky
corner cases may not be handled identically by all front-ends).

### The Back-End

This is the part that converts the syntactic tree to machine code. LLVM is probably the most
popular back-end nowadays, being used in compilers for many languages beyond just C and C++.
Things that typically depend on the back-end are, obviously, code generation and optimization
but also intrinsics.

Note that from a user's point of view, the front-end and the back-end cannot be considered as
totally independent from each other. Some of the features provided by the back-end must be exposed
by the front-end as language extensions (loop unrolling, calling convention, ...). Even intrinsics
usually considered as portable (such as [Intel's](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html)
or [ARM's](https://developer.arm.com/architectures/instruction-sets/intrinsics)) still depend
on the compiler implementing them.

### The Driver

This is the program that brings all the parts of a compiler together, usually as a command line
interface (and there may be several of them, such as`clang.exe` and `clang-cl.exe`). This is out of the scope of this
modest project, but please have a look at [how CMake handles that](https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_COMPILER_FRONTEND_VARIANT.html).

### Variants of a Same Compiler

Identifying both the front-end and the back-end is not always sufficient as there may be several
variants of that pair. Clang/LLVM is a perfect example: MinGW, Intel and ARM all provide compilers
derived from Clang/LVVM.

### A Few Little-Known Combinations

 - [Clang/C2](https://devblogs.microsoft.com/cppblog/clang-with-microsoft-codegen-in-vs-2015-update-1/):
   Clang front-end with MSVC back-end. This was provided as a temporary solution to have modern C++
   support in MSVC while Microsoft reworked their own front-end. This was available in Visual Studio 2015 and 2017.
 - EDG and Intellisense: although MSVC has its own front-end, [Intellisense uses EDG's front-end](https://devblogs.microsoft.com/cppblog/rebuilding-intellisense/).
 - [llvmgcc](https://releases.llvm.org/1.0/docs/CommandGuide/llvmgcc.html): GCC front-end with LLVM back-end. This dates from the days before Clang was available.


The C Library
-------------

This is another widely-encountered mistake: confusing the compiler with the C library. For instance how
you should check for a `float` being a `NaN` (`isnanf()`, `_isnanf()`, `__isnanf()` or `isnan()`?) does
not depend on the compiler but on the C library (although there might be intrinsics to consider too).

A good rule of thumb is that if an identifier's name begins with a single `_` then it belongs to the library
whereas those beginning with two `_` are provided by the compiler (i.e. intrinsics, macros, built-in types, ...).
But keep in mind this is just a rule of thumb, it's not perfect.

License
-------

This software is available either under the zlib license or the Unlicense, pick the one you prefer
out of those two. For details, refer to the [LICENSE](LICENSE) file.
