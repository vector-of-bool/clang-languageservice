#include <libclangxx/index.hpp>

#include <vector>
#include <string>

extern int invoke_real_compiler( int, char** );
extern void do_something_with_translation_unit( const clangxx::TranslationUnit& );

int main( int argc, char** argv )
{
    std::vector<std::string> arguments;
    for ( int i = 0; i < argc; ++i ) { arguments.emplace_back( argv[i] ); }

    clangxx::Index index;
    auto tu = index.createTranslationUnit( arguments );

    // Not all compiler invocations are ones that produce translation units:
    if ( tu.valid() ) { do_something_with_translation_unit( tu ); }

    return invoke_real_compiler( argc, argv );
}


