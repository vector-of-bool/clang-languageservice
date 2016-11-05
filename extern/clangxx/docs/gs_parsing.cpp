#include <libclangxx/index.hpp>

#include <iostream>

int main( int, char** )
{
    clangxx::Index index;
    auto tu = index.parseSourceFile( "myfile.cpp" );

    tu.writePrettyDiagnostics( std::cerr );

    return 0;
}
