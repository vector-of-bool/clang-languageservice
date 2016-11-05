#ifndef LIBCLANGXX_ERRORS_HPP
#define LIBCLANGXX_ERRORS_HPP

#ifndef LIBCLANGXX_STRING_UTILS_HPP
#include <libclangxx/string_utils.hpp>
#endif

#include <exception>
#include <stdexcept>

namespace clangxx
{

class ExceptionBase : public std::exception
{
public:
    template <typename... Args>
    ExceptionBase( Args&&... args ) :
        M_str{ make_string( std::forward<Args>( args )... ) }
    {
    }

    const char* what() const noexcept override
    {
        return M_str.data();
    }

private:
    std::string M_str;
};

#define _DECLARE_EXCEPTION(Name) \
    class Name : public ExceptionBase\
{ \
    using ExceptionBase::ExceptionBase; \
}

_DECLARE_EXCEPTION(UnknownError);
_DECLARE_EXCEPTION(InvalidTranslationUnit);

#undef _DECLARE_EXCEPTION

}

#endif // LIBCLANGXX_ERRORS_HPP
