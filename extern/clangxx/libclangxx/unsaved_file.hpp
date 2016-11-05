#ifndef LIBCLANGXX_UNSAVED_FILE_HPP
#define LIBCLANGXX_UNSAVED_FILE_HPP

#ifndef LIBCLANGXX_CXX1Y_FIX_HPP
#include <libclangxx/cxx1y_fix.hpp>
#endif

#ifndef LIBCLANGXX_LIBCLANG_INDEX_WRAP_HPP
#include <libclangxx/libclang_index_wrap.hpp>
#endif

#include <string>

namespace clangxx
{

/**
 * @brief Represents a similar interface to CXUnsaveFile
 * @todo Doesn't yet provide all the facilities of a CXUnsavedFile. Should
 * work on that!
 */
class UnsavedFile
{
public:
    /**
     * @brief Create an UnsavedFile from the given string
     * @param str The string which represents the source code.
     * @param fname The filename for the unsaved file. Default is an empty
     * string
     */
    UnsavedFile( std::string str, std::string fname = "" ) :
        M_str{ std::move( str ) },
        M_fname{ std::move( fname ) }
    {
        M_reload_handle();
    }

    /// Get a non-const reference to the underlying CXUnsavedFile. I'm
    /// trusting you here!
    auto& handle() { return M_handle; }
    /// Get a const reference to the underlying CXUnsavedFile
    auto& handle() const { return M_handle; }

    /// Get the filename for the unsaved file
    auto& filename() const { return M_fname; }
    /// Get the source code for the unsaved file
    auto& source() const { return M_str; }

    UnsavedFile( const UnsavedFile& other ) :
        M_str{ other.source() },
        M_fname{ other.filename() }
    {
        M_reload_handle();
    }

    UnsavedFile( UnsavedFile&& other ) :
        M_str{ std::move( other.M_str ) },
        M_fname{ std::move( other.M_fname ) }
    {
        M_reload_handle();
    }

    UnsavedFile& operator=( const UnsavedFile& other )
    {
        M_str = other.source();
        M_fname = other.filename();
        M_reload_handle();
        return *this;
    }

    UnsavedFile& operator=( UnsavedFile&& other )
    {
        M_str = std::move( other.M_str );
        M_fname = std::move( other.M_fname );
        M_reload_handle();
        return *this;
    }

private:
    void M_reload_handle()
    {
        M_handle.Contents = M_str.data();
        M_handle.Length = static_cast<unsigned long>(M_str.length());
        M_handle.Filename = M_fname.data();
    }

    /// The underlying CXUnsavedFile
    CXUnsavedFile M_handle;
    /// The source code for the file
    std::string M_str;
    /// The filename for the file
    std::string M_fname;
};

}

#endif // LIBCLANGXX_UNSAVED_FILE_HPP
