#define BOOST_TEST_MODULE ParsingTests tests
#include <tests/testing_header.hpp>

#include <iostream>

BOOST_AUTO_TEST_CASE( Cursors )
{
    using namespace std;

    clangxx::Index index;

    auto tu = index.parseSourceFile( "test_cxx.cpp" );
    BOOST_REQUIRE( tu.valid() );

    for ( auto& cursor : tu.cursor().children() )
    {
        if ( cursor.spelling() == "std" || cursor.spelling() == "auto"
             || cursor.spelling() == "template" )
        {
            BOOST_CHECK( cursor.language() == clangxx::SourceType::CXX );
            BOOST_CHECK( cursor.language()
                         == clangxx::SourceType( CXLanguage_CPlusPlus ) );
        }
        if ( cursor.isPreprocessing() )
        {
            BOOST_CHECK( cursor.language() == clangxx::SourceType::Invalid );
        }
        if ( cursor.kind() == clangxx::Cursor::CXXMethod )
        {
            BOOST_CHECK( cursor.language() == clangxx::SourceType::CXX );
        }
        if ( cursor.spelling() == "somefn" )
        {
            BOOST_CHECK( cursor.kind() == clangxx::Cursor::FunctionDecl );
            BOOST_CHECK( cursor.semanticParent() == tu.cursor() );
        }
    }

    auto cursor = tu.cursorAt( 3, 20 );
    BOOST_CHECK( cursor.spelling() == "somefn" );
    BOOST_CHECK( cursor.kind() == clangxx::Cursor::FunctionDecl );
    auto other_cursor = tu.cursorAt( 3, 19 );
    BOOST_CHECK( cursor == other_cursor );
    BOOST_CHECK( cursor.definition() == cursor );
    BOOST_CHECK( cursor.referenced() == cursor );
    BOOST_CHECK( cursor.USR() == cursor.definition().USR() );
}
