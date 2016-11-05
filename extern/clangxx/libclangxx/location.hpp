#ifndef LIBCLANGXX_LOCATION_HPP
#define LIBCLANGXX_LOCATION_HPP

#include <libclangxx/string_utils.hpp>
#include <libclangxx/libclang_index_wrap.hpp>

namespace clangxx
{

/**
 * @brief Represents a location in a source file. Wraps a CXSourceLocation
 */
class Location
{
public:
    /**
     * @brief Create a Location from the provided CXSourceLocation instance
     * @param location The CXSourceLocation which this object will wrap
     */
    Location( CXSourceLocation location ) : M_location( location )
    {
        clang_getFileLocation( M_location, &M_file, &M_line, &M_col, &M_offset );
    }

    /// Get the underlying CXSourceLocation owned by this object
    auto handle() const { return M_location; }

    /// Format the location in a clang-ly way
    std::string format() const
    {
        return make_string( filename(), ":", M_line, ":", M_col );
    }

    /// Return the filename for the source file that this location is in
    std::string filename() const
    {
        return make_clang_string( clang_getFileName, M_file );
    }

    /// Get the CXFile that this location is contained in
    auto file() const { return M_file; }
    /// Get the location's line number
    auto line() const { return M_line; }
    /// Get the location's column number
    auto col() const { return M_col; }
    /// Get the location's offset into the file
    auto offset() const { return M_offset; }

    /// Returns true if the two locations are equivalent
    bool operator==( const Location& other ) const
    {
        return clang_equalLocations( handle(), other.handle() ) != 0;
    }

private:
    CXFile M_file;
    unsigned M_line{ 0 };
    unsigned M_col{ 0 };
    unsigned M_offset{ 0 };
    CXSourceLocation M_location;
};

/**
 * @brief Wraps CXSourceRange, an half-open range of two source code locations
 */
class SourceRange
{
public:
    /// Default-initializes an invalid range
    SourceRange() = default;

    /// Initializes a range to the given CXRange object
    SourceRange( CXSourceRange range ) :
        M_range( range )
    {}

    /// Returns the location at the start of the range
    Location start() const
    {
        return clang_getRangeStart( M_range );
    }

    /// Returns the location at the end of the range
    Location end() const
    {
        return clang_getRangeEnd( M_range );
    }

    /// Returns the underlying CXRange
    auto& range() const { return M_range; }

    /// Compare two ranges for equality
    bool operator==( const SourceRange& other ) const
    {
        return clang_equalRanges( range(), other.range() ) != 0;
    }

    /// Compare two ranges for inequality
    bool operator!=( const SourceRange& other ) const
    {
        return !(*this == other);
    }

private:
    /// The underlying CXSourceRange
    CXSourceRange M_range;
};
}

#endif // LIBCLANGXX_LOCATION_HPP
