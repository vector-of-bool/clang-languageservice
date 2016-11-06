#ifndef LIBCLANGXX_FILE_HPP
#define LIBCLANGXX_FILE_HPP

#ifndef LIBCLANGXX_STRING_UTILS_HPP
#include <libclangxx/string_utils.hpp>
#endif

#ifndef LIBCLANGXX_LIBCLANG_INDEX_WRAP_HPP
#include <libclangxx/libclang_index_wrap.hpp>
#endif

namespace clangxx
{

/**
 * @brief Wraps the functionality of CXFile
 */
class File
{
public:
    /// Constructs a File object to represent the given CXFile
    File( CXFile file ) :
        M_file{ std::move( file ) },
        M_filename{ make_clang_string( clang_getFileName, M_file ) }
    {}

    /// Get the CXFile associated with the object
    const CXFile& file() const { return M_file; }
    /// Get the filename associated with the file object
    const string& filename() const { return M_filename; }

    /// Get the time_t associated with the file
    time_t time() const { return clang_getFileTime( M_file ); }

private:
    /// The underlying CXFile object
    CXFile M_file;
    /// The name of the file
    std::string M_filename{};
};

}

#endif // LIBCLANGXX_FILE_HPP
