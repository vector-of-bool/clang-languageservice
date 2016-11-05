#ifndef LIBCLANGXX_INDEX_HPP
#define LIBCLANGXX_INDEX_HPP

#ifndef LIBCLANGXX_TRANSLATION_UNIT_HPP
#include <libclangxx/translation_unit.hpp>
#endif


/**
 * @brief Provides classes and functions that will help with parsing and
 * processing of C-family source files.
 */
namespace clangxx
{

/**
 * @brief Wraps a CXIndex instance, which instantiates an instance of the
 * libclang library.
 */
class Index
{
public:
    /// Models a deleter for CXIndex objects
    struct CXIndex_deleter
    {
        /// Calls clang_disposeIndex on the provided CXIndex object
        void operator()( CXIndex ptr )
        {
            if ( ptr ) clang_disposeIndex( ptr );
        }
    };

    /// Default initializes a CXIndex object. Will initialize the index
    /// as if it were created with Index{ false, false };
    constexpr Index() = default;

    /**
     * @brief Initializes an interface to the libclang library
     * @param excludeDeclarationsFromPCH If true, will exclude declarations
     * from PCHs when parsing translation units. Read more in the libclang
     * documentation.
     * @param displayDiagnostics If true, will print diagnostics when parsing
     * translation units. Won't print diagnostics as pretty as would be printed
     * by TranslationUnit::writePrettyDiagnostics.
     */
    Index( bool excludeDeclarationsFromPCH, bool displayDiagnostics ) noexcept
        : M_ptr{ clang_createIndex( excludeDeclarationsFromPCH,
                                    displayDiagnostics ) }
    {
    }

    /// @brief Set the options for the Index object. Read valid options on
    /// the docs for clang_CXIndex_setGlobalOptions
    void setGlobalOptions( unsigned opts )
    {
        clang_CXIndex_setGlobalOptions( ptr(), opts );
    }

    /// @brief Returns the global options for the libclang index. Value comes
    /// straight from clang_CXIndex_getGlobalOptions
    auto globalOptions() { return clang_CXIndex_getGlobalOptions( ptr() ); }

    /**
      * @brief One of the main functions of LibClang++, use it to produce
      * a TranslationUnit instance for the given source file, with the given
      * arguments.
      * @param filename A path to a file to parse.
      * @param args (optional) The compilation arguments to use when parsing the
      * source file
      * @return A TranslationUnit which represents the result of the parse.
      * @see parseSourceString
      * @see createTranslationUnit
      * @see TranslationUnit::createFromSourceFile
      */
    TranslationUnit parseSourceFile( std::string filename,
                                     std::vector<std::string> args = {} )
    {
        std::vector<const char*> argv;
        for ( auto& arg : args )
        {
            argv.emplace_back( arg.data() );
        }
        return TranslationUnit::createFromSourceFile(
            ptr(), filename.data(), int( argv.size() ), argv.data() );
    }

    /**
      * @brief One of hte main functions of LibClang++, use it to produce a
      * TranslationUnit instance for hte given source string with the given
      * arguments.
      * @param source A string for the desired source file.
      * @param args (optional) A sequence of strings that represent the
      * command line arguments to use when parsing the source file.
      * @param type (optional) The type of source code to assume the file is.
      * (We cannot deduce it). By default, will use C++. See
      * TranslationUnit::SourceType
      * @return A TranslationUnit which represents the result of the parse.
      * @see parseSourceFile
      * @see createTranslationUnit
      * @see TranslationUnit::createFromSourceString
      */
    TranslationUnit parseSourceString( const std::string& source,
                                       std::vector<std::string> args = {},
                                       SourceType type = CXX )
    {
        std::vector<const char*> argv;
        for ( auto& arg : args )
        {
            argv.emplace_back( arg.data() );
        }
        return TranslationUnit::createFromSourceString(
            ptr(), source, int( argv.size() ), argv.data(), type );
    }

    /**
      * @brief One of the main functions of LibClang++, use it to create a
      * TranslationUnit from a set of arguments that would go to a clang
      * compiler invocation.
      *
      * This function is the easiest way to parse a source file as if you were
      * the clang compiler.
      * @param args A sequence of command-line arguments which specify all
      * the necessary parsing parameters.
      * @return A TranslationUnit that represents a result of the compiler
      * invocation.
      */
    TranslationUnit createTranslationUnit( std::vector<std::string> args = {} )
    {
        std::vector<const char*> argv;
        for ( auto& arg : args )
        {
            argv.emplace_back( arg.data() );
        }
        return TranslationUnit::createFromArgs( ptr(), int( argv.size() ),
                                                argv.data() );
    }

    /// Returns the underlying CXIndex object
    CXIndex ptr() const { return M_ptr.get(); }

private:
    /// An owning-pointer to the underlying CXIndex object.
    std::unique_ptr<void, CXIndex_deleter> M_ptr{ clang_createIndex( 0, 0 ) };
};
}

#endif // LIBCLANGXX_INDEX_HPP

