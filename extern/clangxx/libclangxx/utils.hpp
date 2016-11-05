#ifndef LIBCLANGXX_UTILS_HPP
#define LIBCLANGXX_UTILS_HPP

#ifndef LIBCLANGXX_CXX1Y_FIX_HPP
#include <libclangxx/cxx1y_fix.hpp>
#endif

#include <functional>
#include <string>
#include <fstream>
#include <memory>

namespace clangxx
{

/**
 * @brief A magically useful macro that provides the semantics and functionality
 * of D's scope(exit).
 * @param name The name for this scope exitter. The name is essential to allow
 * multiple scope-exitter objects to exit within a single scope, as well as
 * being helpful with debugging.
 */
#define clangxx_on_scope_exit( name )                                          \
    scope_exitter name##_scope_exitter;                                        \
    name##_scope_exitter << [&]()->void

/**
 * @brief A tag type used to dispatch to a non-owning function.
 * Used by TranslationUnit::createNonOwning
 */
struct nonowning_t
{
};
/// An instance of nonowning_t used to call various overloads
static nonowning_t nonowning;

/**
 * @brief The scope_exitter struct simply calls a function when it is destroyed.
 * That function is set using the left-shift operator. It is recommended
 * that you should instead use the clangxx_on_scope_exit macro, which
 * provides a nicer syntax to do scope-based execution
 */
struct scope_exitter
{
    ~scope_exitter() { function(); }

    template <typename Func>
    void operator<<( Func func )
    {
        function = std::move( func );
    }

    std::function<void()> function;
};

/**
 * @brief Reads a single line from the given file. Is used in printing
 * diagnostics from a given Location
 * @param fname The path to the file to read
 * @param lineno The line number to read.
 * @return A std::string, hopefully the line requested from the file
 */
inline std::string read_single_line( const std::string& fname, int lineno )
{
    std::ifstream infile{ fname };
    if ( !infile.good() ) return "";

    std::string line;
    while ( std::getline( infile, line ) && lineno-- )
    {
    }
    return line;
}

/// Provides the semantics of C++14's std::make_unique, which isn't provided
/// by libstdc++ yet. This will be replaced once make_unique becomes common in
/// the standard libraries.
template <typename T, typename... Args>
std::unique_ptr<T> make_unique( Args&&... args )
{
    return std::unique_ptr<T>( new T{ std::forward<Args>( args )... } );
}
}

#endif // LIBCLANGXX_UTILS_HPP
