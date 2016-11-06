#ifndef LIBCLANGXX_TRANSLATION_UNIT_HPP
#define LIBCLANGXX_TRANSLATION_UNIT_HPP

#include <libclangxx/cursor.hpp>
#include <libclangxx/unsaved_file.hpp>
#include <libclangxx/errors.hpp>
#include <libclangxx/file.hpp>

#include <algorithm>

namespace clangxx
{

struct DiagnosticGenerator;

/**
 * @brief The TranslationUnit class wraps the CXTranslationUnit class in various
 * helper methods and helper functions, while managing its resources for you.
 */
class TranslationUnit
{
public:
    /// A deleter for CXTranslationUnit objects.
    struct CXTranslationUnit_deleter
    {
        /// Calls `clang_disposeTranslationUnit` on the provided
        /// CXTranslationUnit object
        void operator()( CXTranslationUnit ptr )
        {
            if ( ptr ) clang_disposeTranslationUnit( ptr );
        }
    };

    /// Default-initializes the translation unit to an invalid state
    constexpr TranslationUnit() = default;

    /// Create a non-owning TranslationUnit object to wrap the given
    /// CXTranslationUnit. @note will not call `clang_diposeTranslationUnit`
    /// on the provided object when it leaves scope
    /// @return A non-owning TranslationUnit instance representing the given
    /// CXTranslationUnit
    static TranslationUnit createNonOwning( CXTranslationUnit tu )
    {
        return TranslationUnit{ clangxx::nonowning, tu };
    }

    /**
     * @brief createFromSourceFile creates a TranslationUnit instance from the
     * provided source path and command-line arguments.
     * @param index The CXIndex to create the TranslationUnit from
     * @param path The path to the source file.
     * @param argc The number of arguments in `argv`
     * @param argv The command-line arguments that would go to clang
     * @return A TranslationUnit created from the given arguments
     * @note It's recommended to use the easier-to-use helper method provided
     * in an Index object.
     * @see Index::parseSourceFile
     * @see Index::parseSourceString
     * @see Index::createTranslationUnit
     */
    static TranslationUnit createFromSourceFile( CXIndex index, const char* path, int argc,
                                      const char** argv )
    {
        return TranslationUnit{ clang_createTranslationUnitFromSourceFile(
            index, path, argc, argv, 0, nullptr ) };
    }

    /**
     * @brief createFromASTFile creates a TranslationUnit instance from a Clang
     * AST dump file.
     * @param index The CXIndex to use to create the translation unit
     * @param path The path to the desired Clang AST dump
     * @return A TranslationUnit object created from the given AST dump
     */
    static TranslationUnit createFromASTFile( CXIndex index, const char* path )
    {
        return TranslationUnit{ clang_createTranslationUnit( index, path ) };
    }

    /**
     * @brief defaultEditingOptions returns the value from
     *  clang_defaultEditingTranslationUnitOptions
     * @return The result form `clang_defaultEditingTranslationUnitOptions`
     */
    static std::size_t defaultEditingOptions()
    {
        return clang_defaultEditingTranslationUnitOptions();
    }

    /**
     * @brief parseTranslationUnit creates a TranslationUnit from the given
     * source file and arguments.
     * @param index The CXIndex to use to create the translation unit
     * @param path A path to the source file to parse
     * @param argc The number of arguments in `argv`
     * @param argv The command line-argumetns to pass to clang
     * @param options The options for the parse
     * @return A TranslationUnit created from the given arguments
     * @todo What's the difference between
     * clang_createTranslationUnitFromSourceFile
     * and clang_parseTranslationUnit
     */
    static TranslationUnit parseTranslationUnit( CXIndex index, const char* path, int argc,
                                      const char* const* argv,
                                      unsigned options = 0 )
    {
        return TranslationUnit{ clang_parseTranslationUnit(
            index, path, argv, argc, nullptr, 0, options ) };
    }

    /**
     * @brief createFromSourceString Creates a set of CXUnsavedFile objects
     * and uses them to produce a TranslationUnit object
     * @param index The CXIndex object to create the TranslationUnit from
     * @param source The source string to parse as a source file
     * @param argc The number of arguments in `argv`
     * @param argv The command-line argumetns to pass to clang
     * @param type The source file type (default is C++)
     * @return A TranslationUnit from the given source string and arguments
     * @todo Verify that this works as expected. It seems like a strange way to
     * do things.
     */
    static TranslationUnit createFromSourceString( CXIndex index,
                                        const std::string& source, int argc,
                                        const char** argv,
                                        SourceType type = CXX )
    {
        auto hash = std::hash<std::string>();
        auto fname = std::to_string( hash( source ) );
        if ( type == CXX )
            fname += ".cpp";
        else if ( type == C )
            fname += ".c";
        else if ( type == ObjC )
            fname += ".m";
        else if ( type == ObjCXX )
            fname += ".mpp";
        UnsavedFile tmpfile{ source, fname };
        return TranslationUnit{ clang_createTranslationUnitFromSourceFile(
            index, fname.data(), argc, argv, 1, &tmpfile.handle() ) };
    }

    /**
     * @brief createFromArgs creates a TranslationUnit from a command-line
     * invokation of Clang
     * @param index The CXIndex to use to parse the TarnslationUnit
     * @param argc The number of arguments in `argv`
     * @param argv The command-line arguments that will be sent to Clang
     * @return A TranslationUnit object from the input command-line arguments
     */
    static TranslationUnit createFromArgs( CXIndex index, int argc,
                                const char* const* argv )
    {
        return TranslationUnit{ clang_createTranslationUnitFromSourceFile(
            index, nullptr, argc, argv, 0, nullptr ) };
    }

    /**
     * @brief Returns the source file name
     * @return a std::string representing the translation unit's filename
     */
    string spelling() const
    {
        throwIfInvalid( "Cannot get spelling of null TranslationUnit" );
        return make_clang_string( clang_getTranslationUnitSpelling, ptr() );
    }

    /// Alias of spelling()
    string filename() const { return spelling(); }

    /// Returns the resultl from clang_defaultSaveOptions for this translation
    /// unit
    std::size_t defaultSaveOptions() const
    {
        throwIfInvalid( "Cannot get save options of null TranslationUnit" );
        return clang_defaultSaveOptions( ptr() );
    }

    /**
     * @brief Saves this translation unit to a file
     * @param path The path to the file to create and save to
     * @param options The options to use when saving
     * @todo Document `options` for saving
     */
    void save( std::string path, unsigned options = 0 ) const
    {
        throwIfInvalid( "Cannot save null TranslationUnit" );
        clang_saveTranslationUnit( ptr(), path.data(), options );
    }

    /// Returns the results from clang_defaultReparseOptions for this
    /// translation unit
    std::size_t defaultReparseOptions() const
    {
        throwIfInvalid( "Cannot get reparse options of null TranslationUnit" );
        return clang_defaultReparseOptions( ptr() );
    }

    /**
     * @brief Reparses the translation unit with the given UnsavedFile objects
     * @param files The UnsavedFile objects to use while parsing. Can be empty
     * @param options The options to use while parsing. Default is zero
     * @return An integer representing the result of the reparsing
     */
    std::size_t reparse( const std::vector<UnsavedFile>& files, unsigned options = 0 )
        const
    {
        throwIfInvalid( "Cannot reparse null translation unit!" );
        std::vector<CXUnsavedFile> passfiles;
        // Copy all the handles to the files into the array of CXUnsavedFile:
        std::transform( begin( files ), end( files ),
                        std::back_inserter( passfiles ),
                        []( const UnsavedFile& f )
                        { return f.handle(); } );
        return clang_reparseTranslationUnit(
            ptr(), unsigned( passfiles.size() ), passfiles.data(), options );
    }

    /// @TODO implement the resource usage stuff
    //    struct ResourceUsage
    //    {
    //        struct CXTUResourceUsage_deleter
    //        {
    //            void operator()( CXTUResourceUsage* ptr )
    //            {
    //                clang_disposeCXTUResourceUsage( *ptr );
    //                delete ptr;
    //            }
    //        };

    //        std::unique_ptr<CXTUResourceUsage, CXTUResourceUsage_deleter>
    // handle;
    //    };

    //    auto resourceUsageName() const
    //    {
    //        auto usage = resourceUsage();
    //        return make_clang_string( clang_getTUResourceUsageName(
    // usage.handle.get()->kind ) );
    //    }

    //    ResourceUsage resourceUsage() const
    //    {
    //        ResourceUsage item;
    //        auto handle = clang_getCXTUResourceUsage( ptr() );
    //        std::unique_ptr<CXTUResourceUsage,
    //                        ResourceUsage::CXTUResourceUsage_deleter> ptr;
    //        ptr.reset( new decltype( handle )( handle ) );
    //        return { std::move( ptr ) };
    //    }

    /*
    static TranslationUnit create( CXIndex index, const char* path )
    {
        return TranslationUnit{ clang_createTranslationUnit( index, path )
    };
    }

    static TranslationUnit createFromArgs( CXIndex index, int argc,
                                           const char** argv )
    {
        return TranslationUnit{ clang_createTranslationUnitFromSourceFile(
            index, nullptr, argc, argv, 0, nullptr ) };
    }

    static TranslationUnit createFromSourceFile( CXIndex index,
                                                 const char* path, int argc,
                                                 const char** argv )
    {
        return TranslationUnit{ clang_createTranslationUnitFromSourceFile(
            index, path, argc, argv, 0, nullptr ) };
    }

    static TranslationUnit createFromSourceString( CXIndex index,
                                                   const std::string&
    source,
                                                   int argc, const char**
    argv,
                                                   SourceType type = CXX )
    {
        auto hash = std::hash<std::string>();
        auto fname = std::to_string( hash(source ) );
        if ( type == CXX ) fname += ".cpp";
        else if ( type == C ) fname += ".c";
        else if ( type == ObjC ) fname += ".m";
        else if ( type == ObjCXX ) fname += ".mpp";
        UnsavedFile tmpfile{ source, fname };
        std::cout << "Parsing file: " << fname << std::endl;
        return TranslationUnit{ clang_createTranslationUnitFromSourceFile(
                        index, fname.data(), argc, argv, 1,
    &tmpfile.handle() )
    };
    }

    static TranslationUnit createNonOwning( CXTranslationUnit tu )
    {
        return TranslationUnit{ clangxx::nonowning, tu };
    }

    std::string spelling() const
    {
        return make_clang_string( clang_getTranslationUnitSpelling, ptr() );
    }

    static auto defaultOptions()
    {
        return clang_getTranslationUnitDefaultOptions();
    }
    */

    /**
     * @brief Returns the underlying CXTranslationUnit object
     * @return The CXTranslationUnit associated with the object
     */
    CXTranslationUnit ptr() const { return M_ptr; }

    /**
     * @brief Creates a non-owning copy of this TranslationUnit object
     * @return A non-owning copy of this object
     */
    TranslationUnit nonowning() const { return createNonOwning( ptr() ); }

    /**
     * @brief Returns a DiagnosticGenerator for this TranslationUnit
     * @return A DiagnosticGenerator for this TranslationUnit
     */
    inline DiagnosticGenerator diagnostics();

    /**
     * @brief Writes the diagnostics for this translation unit to the given
     * ostream object. The diagnostics will be formatted in a way similar to
     * how Clang would produce diagnostics, but without any coloring
     * @see writePrettyDiagnostics
     */
    template <typename Strm>
    void writePlainDiagnostics( Strm& strm );

    /**
     * @brief Writes the diagnostics for this translation unit to the given
     * ostream object. The diagnostics will be formatted in a way similar to
     * how Clang would produce diagnostics, but *with* coloring.
     * @see writePlainDiagnostics
     */
    template <typename Strm>
    void writePrettyDiagnostics( Strm& strm );

    /**
     * @brief Returns the Cursor object representing the root of this
     * TranslationUnit object.
     * @return A Cursor object for this translation unit.
     */
    Cursor cursor() const
    {
        throwIfInvalid( "Cannot get cursor of null TranslationUnit" );
        return clang_getTranslationUnitCursor( ptr() );
    }

    /**
     * @brief Get the cursor at a specified source location
     * @param line The line to search at
     * @param column The column to search at
     * @return A Cursor at the position specified
     */
    Cursor cursorAt( unsigned line, unsigned column )
    {
        return cursorAt( locationAt( line, column ) );
    }

    /**
     * @brief Get the cursor at the Location provided
     * @param loc A Location in the source file
     * @return A Cursor at the specified location
     */
    Cursor cursorAt( Location loc )
    {
        throwIfInvalid( "Cannot get cursor from null TranslationUnit" );
        return Cursor{ clang_getCursor( ptr(), loc.handle() ) };
    }

    /**
     * @brief Returns a Location object representing a given location in the
     * TranslationUnit
     * @param line The line in the file
     * @param col The column in the file
     * @return A Location object at the requested location
     */
    Location locationAt( unsigned line, unsigned col )
    {
        throwIfInvalid( "Cannot get location in null TranslationUnit" );
        return Location{ clang_getLocation( ptr(), file( filename() ).file(), line, col ) };
    }

    /**
     * @brief file Get a File object with the specified name associated with
     * this translation unit
     * @param fname The filename to get a file from
     * @return A File object from the specified name
     */
    File file( const std::string& fname ) const
    {
        throwIfInvalid( "Cannot get files of null TranslationUnit" );
        return clang_getFile( ptr(), fname.data() );
    }

    /**
     * @brief Returns true if the TranslationUnit refers to a valid
     * CXTranslationUnit object. If this returns false, many methods will throw
     * an exception
     * @return true if the TranslationUnit refers to a CXTranslationUnit
     */
    bool valid() const { return M_ptr != nullptr; }

    /**
     * @brief Convenience method that throws InvalidTranslationUnit in the case
     * that valid() returns false, using the provided args to construct the
     * exception.
     * @throws InvalidTranslationUnit if valid() is false
     * @param args The arguments to pass to the exception constructor for
     * customizing the exception thrown by this function. (optional)
     */
    template <typename... Args>
    void throwIfInvalid( const Args&... args ) const
    {
        if ( !valid() ) throw InvalidTranslationUnit{ args... };
    }

private:
    /// Creates a TranslationUnit object from the given CXTranslationUnit
    explicit TranslationUnit( CXTranslationUnit tu )
        : M_ptr{ tu }, M_owning{ tu }
    {
    }

    /// Creates a non-owning wrapper around the given CXTranslationUnit
    TranslationUnit( nonowning_t, CXTranslationUnit tu ) : M_ptr{ tu } {}

    /// A non-owning pointer to the translation unit
    CXTranslationUnit M_ptr{ nullptr };
    /// An owning pointer to the TranslationUnit. Will be identical to M_ptr,
    /// unless this TranslationUnit object was created to be non-owning.
    std::unique_ptr<CXTranslationUnitImpl, CXTranslationUnit_deleter> M_owning;
};


/// Represents a set of diagnostic objects
struct DiagnosticSet;

/**
 * @brief The Diagnostic struct represents a clang diagnostic. It wraps a
 * CXDiagnostic instance and manages its lifetime
 */
struct Diagnostic
{
    /**
     * @brief Models a deleter for a CXDiagnostic object
     */
    struct CXDiagnostic_deleter
    {
        /// Calls clang_disposeDiagnostic on the given CXDiagnostic object
        void operator()( CXDiagnostic ptr )
        {
            if ( ptr ) clang_disposeDiagnostic( ptr );
        }
    };

    /// Return the underlying CXDiagnostic object
    CXDiagnostic ptr() const { return M_ptr.get(); }

    /// Replaces this diagnostic with the provided one
    explicit Diagnostic( CXDiagnostic diag ) { assign( diag ); }

    /// Assigned the Diagnostic object to point to a new CXDiagnostic instance
    Diagnostic& operator=( CXDiagnostic diag )
    {
        assign( diag );
        return *this;
    }

    /// Replaces the underlying CXDiagnostic with the provided one
    void assign( CXDiagnostic diag ) { M_ptr.reset( diag ); }

    /// Returns the output text for the diagnostic message
    std::string spelling() const
    {
        return make_clang_string( clang_getDiagnosticSpelling, ptr() );
    }

    /// Returns the severity of the diagnostic
    unsigned severity() const { return clang_getDiagnosticSeverity( ptr() ); }

    /// Returns a textual representation of the diagnostic's severity
    std::string severityText() const
    {
        auto sev = severity();
        return sev == 0 ? "Ignored"
                        : sev == 1 ? "Note"
                                   : sev == 2 ? "Warning"
                                              : sev == 3 ? "Error"
                                                         : sev == 4 ? "Fatal"
                                                                    : "Unknown";
    }

    /// Returns the plain-text representation of the diagnostic, as it is
    /// default formated by clang_formatDiagnostic
    std::string plainText() const
    {
        return make_clang_string( clang_formatDiagnostic, ptr(), 0 );
    }

    /// Returns a full-text representation of the clang diagnostic.
    /// @note If you'd like prettyfull colors to go with your diagnostics,
    /// use prettyText()
    std::string fullText() const
    {
        auto loc = location();
        auto locstr = loc.format();
        auto text = spelling();
        auto opt = option();
        if ( !opt.empty() ) opt = " [" + opt + ']';

        std::string pointer;
        if ( loc.col() != 0 )
        {
            pointer = std::string( loc.col() - 1, ' ' );
            pointer += '^';
        }

        auto level = severityText();

        auto line = read_single_line( loc.filename(), loc.line() - 1 );
        return make_string( locstr, ": ", level, ": ", text, opt, '\n', line,
                            '\n', pointer );
    }

    /// Returns the string representing this diagnostic's category
    std::string category() const
    {
        return make_clang_string( clang_getDiagnosticCategoryText, ptr() );
    }

    /// Returns a string representing the command-line option that enabled
    /// this diagnostic. If the diagnostic is builtin, it returns an empty
    /// string
    std::string option() const
    {
        return make_clang_string( clang_getDiagnosticOption, ptr(), nullptr );
    }

    /// Returns the same thing as fullText, but with coloring.
    std::string prettyText() const
    {
        auto loc = location();
        auto locstr = loc.format();
        auto text = spelling();
        auto opt = option();
        if ( !opt.empty() ) opt = " [" + opt + ']';

        std::string pointer;
        if ( loc.col() != 0 )
        {
            pointer = std::string( loc.col() - 1, ' ' );
            pointer += '^';
        }

        auto level = severityText();
        auto sev = severity();
        if ( sev < 2 )
            level = cyan( level );
        else if ( sev == 2 )
            level = yellow( level );
        else if ( sev > 2 )
            level = red( level );

        auto line = read_single_line( loc.filename(), loc.line() - 1 );
        return make_string( bold( locstr, ": " ), level, ": ", text,
                            cyan( opt ), '\n', line, '\n',
                            bold( green( pointer ) ) );
    }

    /// Returns a Location object that represents the diagnostic's corresponding
    /// source file location
    Location location() const { return clang_getDiagnosticLocation( ptr() ); }

    /// Generates a DiagnosticSet that represents the children of the
    /// diagnostic. Children are things such as backtraces from failed template
    /// instantiations or macro expansions
    inline DiagnosticSet children() const;

private:
    /// An owning pointer to the underlying CXDiagnostic object
    std::unique_ptr<void, CXDiagnostic_deleter> M_ptr;
};


/// Represents an ordered set of diagnostics.
/// @note Returned by Diagnostic::children() for the set of children of a given
/// Clang diagnostic.
/// @note Is valid for use in range-for expressions.
struct DiagnosticSet
{
    /// A deleter for a CXDiagnosticSet
    struct CXDiagnosticSet_deleter
    {
        /// Calls clang_diseposeDiagnosticSet for the given CXDiagnosticSet
        void operator()( CXDiagnosticSet ptr )
        {
            if ( ptr ) clang_disposeDiagnosticSet( ptr );
        }
    };

    /// Initializes a DiagnosticSet from the provided CXDiagnosticSet
    DiagnosticSet( CXDiagnosticSet ptr ) : M_ptr{ ptr } {}

    /// Returns the underlying CXDiagnostic object
    CXDiagnostic ptr() const { return M_ptr.get(); }

    /// Models a RandomAccessIterator that can be used to iterate over the items
    /// in a DiagnosticSet
    struct iterator
    {
        /// Initialized the iterator with the given DiagnosticSet and an index
        iterator( const DiagnosticSet* owner_, unsigned index_ )
            : owner{ owner_ }, index{ index_ }
        {
        }

        /// Returns a NEW Diagnostic object at the iterator's position in
        /// the iterator's corresponding set
        Diagnostic operator*() const
        {
            return Diagnostic{ clang_getDiagnosticInSet( owner->ptr(),
                                                         index ) };
        }

        /// Advance the iterator
        iterator& operator++()
        {
            ++index;
            return *this;
        }

        /// Recedes the iterator
        iterator& operator--()
        {
            --index;
            return *this;
        }

        /// Returns a new iterator at the provided offset from this one
        iterator operator+( int off ) const
        {
            return iterator( owner, index + off );
        }

        /// Returns a new iterator at the provided offset from this one
        iterator operator-( int off ) const
        {
            return iterator( owner, index - off );
        }

        /// Compares two iterators. Will be true if they refer to the same index
        /// and are owned by the same DiagnosticSet
        bool operator==( const iterator& other ) const
        {
            return owner == other.owner && index == other.index;
        }

        /// Compares two iterators, false if they do not compare equal
        bool operator!=( const iterator& other ) const
        {
            return !( *this == other );
        }

        /// The DiagnosticSet that this iterator represents a position in
        const DiagnosticSet* owner;
        /// The index in the DiagnosticSet for this iterator
        unsigned index;
    };

    /// Returns an iterator to the first Diagnostic in this set
    iterator begin() const
    {
        return iterator{ this, 0 };
    }

    /// Returns an iterator to the last Diagnostic in this set
    iterator end() const
    {
        return iterator{ this, clang_getNumDiagnosticsInSet( ptr() ) };
    }

private:
    /// An owning-pointer to the underlying CXDiagnosticSet
    std::unique_ptr<void, CXDiagnosticSet_deleter> M_ptr;
};


DiagnosticSet Diagnostic::children() const
{
    return clang_getChildDiagnostics( ptr() );
}


/// Models an iterable for a TranslationUnit's diagnostics. Is valid
/// in the use of range-for expression.
/// @note Returned by TranslationUnit::diagnostics()
struct DiagnosticGenerator
{
    /// Initializes the generator to produce diagnostics for the given
    /// TranslationUnit object
    explicit DiagnosticGenerator( TranslationUnit& unit )
        : translation_unit{ &unit }
    {
    }

    /// Models a RandomAccessIterator for a DiagnosticGenerator
    struct iterator
    {
        /// Creates an iterator that represents a position in a
        /// DiagnosticGenerator
        iterator( const DiagnosticGenerator* owner, unsigned index_ )
            : generator{ owner }, index{ index_ }
        {
        }

        /// Creates a NEW Diagnostic from the iterator's position in the
        /// DiagnosticGenerator
        Diagnostic operator*() const
        {
            return Diagnostic{ clang_getDiagnostic(
                generator->translation_unit->ptr(), index ) };
        }

        /// Advance the iterator
        iterator& operator++()
        {
            ++index;
            return *this;
        }

        /// Regress the generator
        iterator& operator--()
        {
            --index;
            return *this;
        }

        /// Returns a new iterator at the provided offset from the iterator
        iterator operator+( int off ) const
        {
            return iterator( generator, index + off );
        }

        /// Returns a new iterator at the provided offset from the given one
        iterator operator-( int off ) const
        {
            return iterator( generator, index - off );
        }

        /// Compares two iterators. They are equal if they are both owned
        /// by the same DiagnosticGenerator and have the same index
        bool operator==( const iterator& other ) const
        {
            return generator == other.generator && index == other.index;
        }

        /// Compares to iterators for inequality
        bool operator!=( const iterator& other ) const
        {
            return !( *this == other );
        }

        /// The generator that the iterator represents a position in
        const DiagnosticGenerator* generator;
        /// The offset in the generator's diagnostics
        unsigned index;
    };

    /// Returns an iterator to the first Diagnostic in the generator
    iterator begin() const
    {
        return iterator{ this, 0 };
    }

    /// Returns an iterator to the last Diagnostic in the generator
    iterator end() const
    {
        return iterator{ this,
                         clang_getNumDiagnostics( translation_unit->ptr() ) };
    }

    /// The TranslationUnit that the generator is linked to
    TranslationUnit* translation_unit;
};


inline DiagnosticGenerator TranslationUnit::diagnostics()
{
    throwIfInvalid( "Cannot get diagnostics of null TranslationUnit" );
    return DiagnosticGenerator{ *this };
}


template <typename Strm>
void TranslationUnit::writePlainDiagnostics( Strm& strm )
{
    for ( const auto& diag : diagnostics() )
    {
        strm << diag.fullText() << '\n';
        for ( const auto& c : diag.children() )
        {
            strm << c.fullText() << '\n';
        }
    }
}


template <typename Strm>
void TranslationUnit::writePrettyDiagnostics( Strm& strm )
{
    for ( const auto& diag : diagnostics() )
    {
        strm << diag.prettyText() << '\n';
        for ( const auto& c : diag.children() )
        {
            strm << c.prettyText() << '\n';
        }
    }
}
}

#endif // LIBCLANGXX_TRANSLATION_UNIT_HPP
