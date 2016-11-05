/*
 * Copyright (c) 2014, Colby Pike
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of LibClang++ nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL COLBY PIKE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#ifndef LIBCLANGXX_DOCS_HPP
#define LIBCLANGXX_DOCS_HPP

/**
  * @mainpage LibClang++
  *
  * LibClang++ is a C++ wrapping around <a
  * href="http://clang.llvm.org/doxygen/group__CINDEX.html">libclang</a>, an
  * easy-to-use API for parsing source code base on the Clang parser.
  *
  * LibClang++ is written with C++14, and (at the time of this writing) is
  * able to be compiled with Clang 3.4 and GCC 4.8.1. Future compilers will most
  * likely be supported as C++1y features are implemented in them.
  *
  * LibClang++ attempts to make it as easy as possible to parse C-like source
  * code quickly and abstractly on top of libclang.
  *
  * There are three main goals of the project:
  *
  * - @ref coverage "To provide a full C++ interface to LibClang", abstracting away many
  *     of the plumbing and boilerplate required to use libclang in C++. If you
  *     see any libclang API that isn't supplanted by LibClang++, PLEASE submit
  *     a ticket. That's what helps make the API better! If you think any
  *     technique or API desisions are obtuse or clumsy, PLEASE discuss it.
  *     That's what also helps make the API better!
  * - @ref examples "To make it as easy as possible to quickly parse and analyze source code".
  *     If LibClang++ isn't doing that for you, PLEASE submit a ticket! The
  *     API's goal is to make it easy to parse source code! If it's not making
  *     that easy, then the API is *wrong*, so PLEASE submit a ticket!
  * - @ref doc_status "To be thoroughly documented!" One thing that open source
  *     suffers is poorly documented software. LibClang++ has (or should have)
  *     all of its components documented, both public and private. If you see
  *     any component that is poorly/not documented, PLEASE submit a ticket!
  *     That's what helps make the API better!
  *
  * It's documentation attempts to be as complete
  * and thorough as possible, and all classes, functions, and methods should
  * be documented. If you spot a component that isn't documented sufficiently,
  * then PLEASE submit a ticket.
  */


/**
  * @page coverage libclang Coverage
  *
  * @tableofcontents
  *
  * @section cov_intro Introduction
  *
  * LibClang++ attempts to provide all the functionality of libclang in a C++
  * interface. <a href="http://clang.llvm.org/doxygen/group__CINDEX.html">Read
  * the documentation of libclang here.</a> libclang is part of the Clang
  * project, which is part of the LLVM project.
  * <a href="http://clang.llvm.org">Read about the Clang toolchain here</a>
  * and <a href="http://llvm.org">the LLVM project here.</a>
  *
  * This specific page documents coverage of the libclang API and specifies
  * how libclang code can be written using nothing but standard C++. Although
  * LibClang++ depends on libclang and the libclang headers, LibClang++ hides
  * all of the nitty-gritty details behind an easy-to-use C++14 API.
  *
  * @ref getting_started "See the Getting Started page to learn how to use the API in your project"
  *
  *
  */


/**
  * @page examples LibClang++ Examples
  *
  * @tableofcontents
  *
  * @section ex_parsing Parsing a Source File and Printing Diagnostics
  *
  * This example opens a file "`myfile.cpp`" and parses it with libclang. After
  * parsing, it prints the diagnostics in a "pretty" format with colorring
  * and column indicators, as well as some hints.
  *
  * @includelineno gs_parsing.cpp
  *
  * @section ex_intercepting Intercepting a Command Line Invocation
  *
  * Sometimes you may want to write a meta-compiler, that compiles sources
  * after inspecting them. This may be useful by scanning code during the build
  * process to find certain constructs that one may be looking out for.
  *
  * @includelineno ex_intercepting.cpp
  *
  * @section ex_find_fn Finding Calls to a Certain Function
  *
  * Sometimes you'll want to see where a function is being called for debugging
  * and diagnosing purposes. That's also easy:
  *
  * @includelineno ex_finding.cpp
  *
  */


/**
  * @page doc_status Documentation Status
  *
  * Documentation of the LibClang++ classes and functions is always a work in
  * progress, and must constantly be improved with usage examples and
  * explainations of the various designs it contains.
  *
  * If you, user, find anything in these documents that is incomplete or could
  * be improved, *please* feel free to submit a ticket. Incomplete documentation
  * is just as much an issue as incomplete functionality.
  *
  * This page will later contain information on the current status of
  * documentation for the project. Come back later to find more here!
  */


/**
  * @page general_help Help and Tutorials
  *
  * There are several pages in this documentation to help working with
  * LibClang++
  *
  * - @ref installing "Installing LibClang++ is documented on this page." It
  *     shows the best way to install it from source.
  * - @ref getting_started "The Getting Started" page shows how you can get
  *     started using LibClang++ in a very basic way.
  * - @ref examples "The examples page" shows a variety of examples of using
  *     LibClang++ to do a variety of tasks.
  */


/**
  * @page installing Installing LibClang++
  *
  * @tableofcontents
  *
  * @section installing_intro Introduction
  *
  * LibClang++ is a header-only library, but uses CMake to make configuring
  * your projects easy. You'll need CMake installed in order to install from the
  * source tree.
  *
  * @section installing_from_source Installing from Source
  *
  * @subsection installing_getting Getting the Source
  *
  * Getting the source is easy if you have Git:
  *
  * ~~~
  * git clone git@bitbucket.org:SirSymbolTable/libclang.git libclangxx
  * ~~~
  *
  * This will download the source tree and place it in `libclangxx` underneath
  * your current directory. By default, you'll probably want to use the `master`
  * branch. If you like living on the edge, checkout the `release` or `develop`
  * branch. To check out a specific development work, check out its feature
  * branch.
  *
  * If you got the source in some other way, you'll be stuck with the version
  * you've got, but it should work just fine as long as the source tree is
  * completely intact.
  *
  * @subsection installing_building Building the Source Tree
  *
  * At the root of the source tree is the main `CMakeLists.txt` file. From a
  * build directory, invoke CMake:
  *
  * ~~~
  * cmake <path-to-source-tree>
  * ~~~
  *
  * If CMake was not able to finish, @ref installing_trouble "check here".
  *
  * If CMake finished without error, try building the unit tests with your build
  * tool. On many systems, this will probably be GNU Make:
  *
  * ~~~
  * make -j4
  * ~~~
  *
  * If you haven't configured the build to build the tests, this command will
  * probably do nothing.
  *
  * If you have doxygen installed on your system and available on your `PATH`,
  * you'll have the '`docs`' target available. Building this target will generate
  * the documentation you are currently reading:
  *
  * ~~~
  * make docs
  * ~~~
  *
  * If you were able to build the unit tests, you can run them by building the
  * '`test`' target:
  *
  * ~~~
  * make test
  * ~~~
  *
  * @subsection installing_installing Invoking the Install Target
  *
  * If you wanted LibClang++ to get installed to a particular directory, you'll
  * need to manaully override the `CMAKE_INSTALL_PREFIX` variable in the CMake
  * cache.
  *
  * To do the installation, build the `install` target:
  *
  * ~~~
  * make install
  * ~~~
  *
  * You may need elevated privileges in order to do this, but all should work
  * as expected.
  *
  * @subsection installing_uninstalling Uninstalling LibClang++
  *
  * It's important to be able to undo anything you've done. In our case, we've
  * generated an `uninstall` target that will automatically remove every file
  * that was installed during the most recent `install` invocation:
  *
  * ~~~
  * make uninstall
  * ~~~
  *
  * Similar to the `install` target, building this target may require elevated
  * privileges.
  *
  *
  * @section installing_trouble CMake Didn't Work!
  *
  * This is entirely possible and may be caused by a variety of factors. The
  * most likely is that CMake wasn't able to find your `llvm-config` program.
  * If CMake complained about `llvm-config`, you'll want to manually set the
  * cache variable `LLVM_CONFIG` to point to the full path to the llvm-config
  * executable. CMake uses it to get information about libclang as well as well
  * as your LLVM version information.
  *
  * If pointing the path to `llvm-config` didn't seem to to work, try submitting
  * a ticket to help make the build system better.
  *
  */


/**
  * @page getting_started Getting Started
  *
  * @tableofcontents
  *
  *
  * @section gs_intro Introduction
  *
  * LibClang++ uses CMake to install and configure itself. If you have a project
  * using CMake and libclang installed, it is easy to get started.
  *
  *
  * @section gs_prereq Prerequisites
  *
  * In order to get started with LibClang++, there are a few pieces of software
  * you will need available first.
  *
  *
  * @subsection gs_cmake CMake
  *
  * <a href="http://cmake.org">CMake</a> is a extremely powerful cross-platform
  * "meta" build system. It generates build files for other build systems,
  * including Visual Studio, CodeBlocks, GNU Make, Ninja, and many others.
  *
  * LibClang++ is known to work with CMake 2.8.11, but may work with older
  * versions perfectly well. @todo Determine a correct minimum CMake version.
  *
  *
  * @subsection gs_clang Clang or GCC
  *
  * <a href="http://clang.llvm.org">Clang</a> is an excellect compiler for
  * C-family languages, including C++ and Objective-C. LibClang++ header files
  * are known to compile without error in Clang 3.4 (The most recent revision).
  *
  * <a href="http://gcc.gnu.org">GCC</a> is the classic open source compiler
  * for many systems. LibClang++ headers are known to compile without error
  * in GCC 4.8.1.
  *
  * LibClang++ cannot compile in in present versions of MSVC (18),
  * but should compile fine once C++11 and C++14 adoption are fairly complete.
  *
  * If you think your compiler should be able to compile LibClang++ code
  * correctly, PLEASE submit a ticket.
  *
  *
  * @subsection gs_libclang libclang
  *
  * libclang is a library based on <a href="http://clang.llvm.org">Clang</a>
  * that provides a stable and easy-to-use C API wrapper around Clang's
  * parsers. LibClang++ is header only, and provides wrappings around libclang.
  *
  * libclang can be built from source, or installed as a package on many Linux
  * distributions.
  *
  * @note When installing LibClang++ from source, it searches for `llvm-config`
  * on several different default paths in order for it to do the installation
  * correctly. If CMake cannot find the `llvm-config` program, you may need to
  * specify it manually.
  *
  * @todo Have CMake search for LLVM from within the *-config.cmake file, so
  * that it is more portable.
  *
  *
  * @section howto_cmake Using CMake to Configure your Targets with LibClang++
  *
  * With CMake installed, you should now be ready to work with LibClang++.
  *
  * To start with LibClang++, you'll need to install it correctly. If you don't
  * have a nice package to install it with, you'll want to read up on
  * @ref installing "installing it from source".
  *
  * @todo Create LibClang++ Linux packages, which are much more awesome than
  * just a plain install target.
  *
  * Once you have LibClang++ installed, use a single CMake line to introduce
  * it to your project:
  *
  * ~~~
  * find_package(LibClangXX)
  * ~~~
  *
  * Tada! Now you can use LibClang++ in your targets. @note You may need to
  * add directories to your package search path if you installed LibClang++ to
  * a non-standard location.
  *
  * Using LibClang++ with your targets is simple:
  *
  * ~~~
  * targets_link_libraries(<your-target-here> libclangxx)
  * ~~~
  *
  * This will add the necessary libraries and compilation flags to your project
  * for it to compile with LibClang++ and link in the necessary Clang libraries.
  *
  * @note LibClang++ requires support for C++1y, and (depending on your CMake
  * version) you may need to enable it manually with a compiler switch, such
  * as `-std=c++1y`. Newer versions of CMake will automatically enable C++1y
  * support when you link to LibClang++.
  *
  * @warning LibClang++ requires proper C++1y support.
  * This may cause issues if you are writing invalid C++1y code, so be careful.
  * (Code involing const methods and constexpr may change compile-time behavior.
  * This is a known issue in some versions of libstdc++ which depend on
  * `constexpr` to imply `const` on some class methods.)
  *
  * @warning C++1y, Clang, and libstdc++ don't always play nicely. There's a
  * common issue in `<cstdio>` involving the deprecated `gets` function. If you
  * are experiencing this issue, simply make sure that you include
  * `<libclangxx/cxx1y_fix.hpp>` or some other LibClang++ header before
  * any other standard library headers.
  *
  * If your code can compile with these two lines added, then you are ready to
  * get started using LibClang++. Read on!
  *
  * @section gs_basic Basic Usage
  *
  * @subsection gs_including Include Directories
  *
  * All LibClang++ headers are included underneath the libclangxx/ directory.
  * For example:
  *
  * ~~~
  * #include <libclangxx/index.hpp>
  * #include <libclangxx/cursor.hpp>
  * ~~~
  *
  * LibClang++ headers must be included using angle brackets, not parentheses.
  *
  * @subsection gs_create_index Creating an Index
  *
  * libclang uses a CXIndex object to instatiate the library and the Clang
  * parsers. LibClang++ uses the class clangxx::Index. To start working with
  * Clang++, instantiate this class. @note The index holds all of the library's
  * resources, so it must outlive all other objects that come from it.
  *
  * There's only one header we need for now:
  *
  * @dontinclude gs_parsing.cpp
  * @line include
  *
  * This include exposes much of the LibClang++ API, including clangxx::Index.
  * Creating one is simple:
  *
  * @skip clangxx::Index
  * @line clangxx::Index
  *
  * The index object will automatically free all resources consumed by the
  * library once it is destroyed.
  *
  * Once you have an index, you can generate a TranslationUnit object from it,
  * which is the main way to parse files. Parsing is simple:
  *
  * @line parse
  *
  * You've now parsed the source file, and the data for the parse is stored in
  * the TranslationUnit object `tu`. But you still haven't done anything with
  * it. One of the things you may want to do is print the file's diagnostics.
  * LibClang++ provides a convenience function that automatically writes the
  * diagnostics for a translation unit in a similar way to how Clang itself
  * would. It's only a single function call:
  *
  * @line Pretty
  *
  * The clangxx::TranslationUnit::writePrettyDiagnostics method will write the
  * diagnostics for the translation unit to the provided C++ ostream object, in
  * this case `stderr`. There's a similar method,
  * clangxx::TranslationUnit::writePlainDiagnostics, which rights identical data,
  * but without any of the nice coloring that makes the diagnostics easier to
  * read.
  *
  * If we put these all together, we have this tiny program:
  *
  * @includelineno gs_parsing.cpp
  *
  * That's a pretty good size, considering what is going on internally.
  * Unfortunately, the call to writePrettyDiagnostics requires that the
  * associated translation unit be a valid object. You can check it's validity
  * with the clangxx::TranslationUnit::valid() method. Many methods require that
  * the associated objects be valid. If the object is not valid, they will throw
  * an exception instead of silently failing.
  *
  *
  * @subsection gs_parsing_opt Parsing with Options
  *
  * Sometimes it isn't enough to just parse a file normally. Sometimes the file
  * depends on include directories or other compiler switches. These can be
  * passed in to the parsing functions easily:
  *
  * ~~~
  * auto tu = index.parseSourceFile( "myfile.cpp", { "-std=c++11", "-I/SomeDir/include" } );
  * ~~~
  *
  * Most valid Clang command-line options are completely valid to pass in to
  * these functions. Ones that require/generate output are ignored. See the
  * libclang documentation for more information
  *
  * @subsection gs_parsing_cmd Parsing a Command-Line Invocation
  *
  * Sometimes you'll only have the command line arguments, and no files. Parsing
  * is also very simple. Simple get the arguments into a
  * `std::vector<std::string>` and pass them like this:
  *
  * ~~~
  * auto tu = index.createTranslationUnit( my_arguments );
  * ~~~
  *
  * Again, see the libclang documentation to see what arguments are ignored or
  * invalid.
  *
  */


/**
  * @page licensing Licensing of LibClang++
  *
  * LibClang++ can be used under the terms of the BSD license, to permit
  * maximum freedom of all users for almost all purposes.
  *
  *
  *
 * Copyright (c) 2014, Colby Pike
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of LibClang++ nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL COLBY PIKE BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#endif // LIBCLANGXX_DOCS_HPP
