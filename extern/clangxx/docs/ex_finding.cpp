#include <libclangxx/index.hpp>

#include <string>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

using namespace clangxx;

namespace
{
std::vector<Location> find_bad_function_calls( const TranslationUnit&,
                                               const std::string& funcname );
}

int main( int argc, char** argv )
{
    if ( argc < 3 )
    {
        cerr << "Expected a filename and function name!" << endl;
        return 1;
    }

    std::string filename = argv[1];
    std::string funcname = argv[2];

    clangxx::Index index;
    // Parse a source file in C++1y mode:
    auto tu = index.parseSourceFile( filename, { "-std=c++1y" } );

    // Find the bad function in this translation unit:
    auto locations = find_bad_function_calls( tu, funcname );

    for ( const auto& loc : locations )
    {
        cout << "Found call to \"" << funcname << "\": " << loc.format() << endl;
    }

    return 0;
}

namespace
{

std::vector<Location> find_bad_cursors( const Cursor& c,
                                        const std::string& funcname )
{
    std::vector<Location> found;
    // If this is a function call to the given function, add it to
    // our list of call sites:
    if ( c.kind() == c.CallExpr && c.spelling() == funcname )
    {
        found.emplace_back( c.location() );
    }
    // Collect the call sites from our children as well:
    for ( const auto& child : c.children() )
    {
        auto res = find_bad_cursors( child, funcname );
        std::copy( begin( res ), end( res ), std::back_inserter( found ) );
    }
    // Return the results for this cursor and all of our children
    return found;
}

std::vector<Location> find_bad_function_calls( const TranslationUnit& tu,
                                               const std::string& funcname )
{
    return find_bad_cursors( tu.cursor(), funcname );
}

} // End of anonymous-namespace
