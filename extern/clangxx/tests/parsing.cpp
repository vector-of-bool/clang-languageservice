/**
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

#define BOOST_TEST_MODULE ParsingTests tests
#include <tests/testing_header.hpp>

BOOST_AUTO_TEST_CASE( BasicParsing )
{
    clangxx::Index index{};

    auto inline_source = R"(
                  int h;
    extern void dothing();
                  )";

    auto inline_cxx11 = R"(
    auto h = 9;
    auto do_thing() -> decltype(8);
    template <typename... >
    void fn();
                        )";

    auto inline_cxx1y = R"(
    auto h = 9;
    auto get_number() { return 9; }
    auto fn = [](auto h){ return h; };
    )";

    auto parse_string = [&]
    { auto tr = index.parseSourceString( inline_source ); };

    auto parse_cxx11_string = [&]
    { auto tr = index.parseSourceString( inline_cxx11 ); };

    auto parse_cxx1y_string = [&]
    { auto tr = index.parseSourceString( inline_cxx1y ); };

    BOOST_CHECK_NO_THROW( parse_string() );
    BOOST_CHECK_NO_THROW( parse_cxx11_string() );
    BOOST_CHECK_NO_THROW( parse_cxx1y_string() );
    BOOST_CHECK_NO_THROW( index.parseSourceFile( "test_cxx.cpp" ) );
    BOOST_CHECK_NO_THROW(
        index.parseSourceFile( "test_cxx11.cpp", { "-std=c++11" } ) );
    BOOST_CHECK_NO_THROW(
        index.parseSourceFile( "test_cxx1y.cpp", { "-std=c++1y" } ) );
    BOOST_CHECK_NO_THROW(
        index.createTranslationUnit( { "-std=c++1y", "test_cxx1y.cpp" } ) );

    auto tu = index.parseSourceFile( "test_cxx.cpp" );
    BOOST_CHECK(tu.valid());
}
