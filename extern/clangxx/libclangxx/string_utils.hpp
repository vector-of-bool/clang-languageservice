#ifndef LIBCLANGXX_STRING_UTILS_HPP
#define LIBCLANGXX_STRING_UTILS_HPP

#include <libclangxx/utils.hpp>

#include <sstream>

namespace clangxx
{

/**
 * @brief Takes a function and set of arguments that will return a CXString,
 * and translates the resulting CXString into a std::string
 *
 * @param func The callable object, which - when invoked - will return
 * a CXString object.
 * @param args The arguments to forward to `func`
 *
 * @tparam StrType The string type which to return. Must be explicitly
 * constructible from a `const char*` (optional, defaults to std::string)
 */
template <typename StrType = std::string, typename Func, typename... Args>
StrType make_clang_string( Func&& func, Args&&... args )
{
    auto str = func( std::forward<Args>( args )... );
    clangxx_on_scope_exit( str )
    {
        clang_disposeString( str );
    };

    auto ptr = clang_getCString( str );
    return StrType{ ptr ? ptr : "" };
}

namespace detail
{

/// Base case for make_string_impl
inline void make_string_impl( std::stringstream& ) {}

/// Formats a the given arguments and injects them into the stringstream object
/// @param strm The std::stringstream to format the arguments into
/// @param arg The head argument to go into std::stringstream
/// @param args The tail arguments to go into std::stringstream
/// @internal make_string_impl
template <typename T, typename... Args>
void make_string_impl( std::stringstream& strm, const T& arg,
                       const Args&... args )
{
    strm << arg;
    make_string_impl( strm, args... );
}
}

/**
 * @brief Creates a std::string from a set of input arguments, by formatting
 * and concatinating each argument sequentially.
 * @return a std::string created from the given arguments.
 * @note No whitespace is injected or appended. All formatting must be provided
 * in the arguments.
 */
template <typename... Args>
std::string make_string( const Args&... args )
{
    std::stringstream strm;
    detail::make_string_impl( strm, args... );
    return strm.str();
}


/// ANSI control code to reset colors
constexpr static const char* reset_colors = "\x1b[39;49m";
/// ANSI control code for red
constexpr static const char* red_color = "\x1b[31m";
/// ANSI control code for green
constexpr static const char* green_color = "\x1b[32m";
/// ANSI control code for yellow
constexpr static const char* yellow_color = "\x1b[33m";
/// ANSI control code for blue
constexpr static const char* blue_color = "\x1b[34m";
/// ANSI control code for magenta
constexpr static const char* magenta_color = "\x1b[35m";
/// ANSI control code for cyan
constexpr static const char* cyan_color = "\x1b[36m";

/// Produces a string of the given color from the given arguments
/// @see make_string
/// @see red_color and friends
/// @warning Beware nesting colors. ANSI color controls are fragile!
template <typename... Args>
std::string colorize( const char* code, const Args&... args )
{
    return code + make_string( args... ) + reset_colors;
}

#define _DECL_COLOR_TEMPLATE( color )                                          \
    template <typename... Args>                                                \
    std::string color( const Args&... args )                                   \
    {                                                                          \
        return colorize( color##_color, args... );                             \
    }

_DECL_COLOR_TEMPLATE( red )
_DECL_COLOR_TEMPLATE( green )
_DECL_COLOR_TEMPLATE( yellow )
_DECL_COLOR_TEMPLATE( blue )
_DECL_COLOR_TEMPLATE( magenta )
_DECL_COLOR_TEMPLATE( cyan )

#undef _DECL_COLOR_TEMPLATE

/// ANSI control code to reset text style
constexpr static const char* reset_styles = "\x1b[22m";
/// ANSI control code for a bold text style
constexpr static const char* bold_style = "\x1b[1m";

/// Produces a string of the given style from the given arguments
/// @see make_string
/// @see bold_style and friends
/// @warning Beware nesting styles. ANSI control codes are fragile!
template <typename... Args>
std::string stylize( const char* style, const Args&... args )
{
    return style + make_string( args... ) + reset_styles;
}

#define _DECL_STYLE_TEMPLATE( style )                                          \
    template <typename... Args>                                                \
    std::string style( const Args&... args )                                   \
    {                                                                          \
        return stylize( style##_style, args... );                              \
    }

_DECL_STYLE_TEMPLATE(bold)

#undef _DECL_STYLE_TEMPLATE
}

#endif // LIBCLANGXX_STRING_UTILS_HPP
