#ifndef LIBCLANGXX_CURSOR_HPP
#define LIBCLANGXX_CURSOR_HPP

#include <libclangxx/location.hpp>

#include <cstdint>

#include <vector>
#include <string>

namespace clangxx
{

using std::string;

/// The types of Source code LibClang is parsing
enum SourceType
{
    /// Unknown/invalid source type
    Invalid = CXLanguage_Invalid,
    /// C++ source code
    CXX = CXLanguage_CPlusPlus,
    /// C source code
    C = CXLanguage_C,
    /// Objective-C source code
    ObjC = CXLanguage_ObjC,
    /// Objective-C++ source code
    ObjCXX
};

class CursorGenerator;
class TranslationUnit;

/**
 * @brief The Cursor class makes it easy to walk and examine an AST. This class
 * wraps a CXCursor object.
 */
class Cursor
{
public:
    // This hurt to type. There's probably a better way to get them all here,
    // but I'm way to lazy to figure it out. If you See a cursor kind that
    // isn't declared, please file a report!
    /**
     * @brief Represents a CXCursorKind. The values here are the same as
     * their respective `CXCursor_*` values. See the libclang docs to see what
     * each of these means individually. Doxygen parses this enum strangely,
     * so you may want to have a look at the source for it for yourself.
     */
    enum Kind
    {
#define D( name ) name = CXCursor_##name
        D( UnexposedDecl ),
        D( StructDecl ),
        D( UnionDecl ),
        D( ClassDecl ),
        D( EnumDecl ),
        D( FieldDecl ),
        D( EnumConstantDecl ),
        D( FunctionDecl ),
        D( VarDecl ),
        D( ParmDecl ),
        D( ObjCInterfaceDecl ),
        D( ObjCCategoryDecl ),
        D( ObjCProtocolDecl ),
        D( ObjCPropertyDecl ),
        D( ObjCIvarDecl ),
        D( ObjCInstanceMethodDecl ),
        D( ObjCClassMethodDecl ),
        D( ObjCImplementationDecl ),
        D( ObjCCategoryImplDecl ),
        D( TypedefDecl ),
        D( CXXMethod ),
        D( Namespace ),
        D( LinkageSpec ),
        D( Constructor ),
        D( Destructor ),
        D( ConversionFunction ),
        D( TemplateTypeParameter ),
        D( NonTypeTemplateParameter ),
        D( TemplateTemplateParameter ),
        D( FunctionTemplate ),
        D( ClassTemplate ),
        D( ClassTemplatePartialSpecialization ),
        D( NamespaceAlias ),
        D( UsingDirective ),
        D( UsingDeclaration ),
        D( TypeAliasDecl ),
        D( ObjCSynthesizeDecl ),
        D( ObjCDynamicDecl ),
        D( CXXAccessSpecifier ),
        D( FirstDecl ),
        D( LastDecl ),
        D( FirstRef ),
        D( ObjCSuperClassRef ),
        D( ObjCProtocolRef ),
        D( ObjCClassRef ),
        D( TypeRef ),
        D( CXXBaseSpecifier ),
        D( TemplateRef ),
        D( NamespaceRef ),
        D( MemberRef ),
        D( LabelRef ),
        D( OverloadedDeclRef ),
        D( VariableRef ),
        D( LastRef ),
        D( FirstInvalid ),
        D( InvalidFile ),
        D( NoDeclFound ),
        D( NotImplemented ),
        D( InvalidCode ),
        D( LastInvalid ),
        D( FirstExpr ),
        D( UnexposedExpr ),
        D( DeclRefExpr ),
        D( MemberRefExpr ),
        D( CallExpr ),
        D( ObjCMessageExpr ),
        D( BlockExpr ),
        D( IntegerLiteral ),
        D( FloatingLiteral ),
        D( ImaginaryLiteral ),
        D( StringLiteral ),
        D( CharacterLiteral ),
        D( ParenExpr ),
        D( UnaryOperator ),
        D( ArraySubscriptExpr ),
        D( BinaryOperator ),
        D( CompoundAssignOperator ),
        D( ConditionalOperator ),
        D( CStyleCastExpr ),
        D( CompoundLiteralExpr ),
        D( InitListExpr ),
        D( AddrLabelExpr ),
        D( StmtExpr ),
        D( GenericSelectionExpr ),
        D( GNUNullExpr ),
        D( CXXStaticCastExpr ),
        D( CXXDynamicCastExpr ),
        D( CXXReinterpretCastExpr ),
        D( CXXConstCastExpr ),
        D( CXXFunctionalCastExpr ),
        D( CXXTypeidExpr ),
        D( CXXBoolLiteralExpr ),
        D( CXXNullPtrLiteralExpr ),
        D( CXXThisExpr ),
        D( CXXThrowExpr ),
        D( CXXNewExpr ),
        D( CXXDeleteExpr ),
        D( UnaryExpr ),
        D( ObjCStringLiteral ),
        D( ObjCEncodeExpr ),
        D( ObjCSelectorExpr ),
        D( ObjCProtocolExpr ),
        D( ObjCBridgedCastExpr ),
        D( PackExpansionExpr ),
        D( SizeOfPackExpr ),
        D( LambdaExpr ),
        D( ObjCBoolLiteralExpr ),
        D( ObjCSelfExpr ),
        D( LastExpr ),
        D( FirstStmt ),
        D( UnexposedStmt ),
        D( LabelStmt ),
        D( CompoundStmt ),
        D( CaseStmt ),
        D( DefaultStmt ),
        D( IfStmt ),
        D( SwitchStmt ),
        D( WhileStmt ),
        D( DoStmt ),
        D( ForStmt ),
        D( GotoStmt ),
        D( IndirectGotoStmt ),
        D( ContinueStmt ),
        D( BreakStmt ),
        D( ReturnStmt ),
        D( GCCAsmStmt ),
        D( AsmStmt ),
        D( ObjCAtTryStmt ),
        D( ObjCAtCatchStmt ),
        D( ObjCAtFinallyStmt ),
        D( ObjCAtThrowStmt ),
        D( ObjCAtSynchronizedStmt ),
        D( ObjCAutoreleasePoolStmt ),
        D( ObjCForCollectionStmt ),
        D( CXXCatchStmt ),
        D( CXXTryStmt ),
        D( CXXForRangeStmt ),
        D( SEHTryStmt ),
        D( SEHExceptStmt ),
        D( SEHFinallyStmt ),
        D( MSAsmStmt ),
        D( NullStmt ),
        D( DeclStmt ),
        D( OMPParallelDirective ),
        D( LastStmt ),
        D( TranslationUnit ),
        D( FirstAttr ),
        D( UnexposedAttr ),
        D( IBActionAttr ),
        D( IBOutletAttr ),
        D( IBOutletCollectionAttr ),
        D( CXXFinalAttr ),
        D( CXXOverrideAttr ),
        D( AnnotateAttr ),
        D( AsmLabelAttr ),
        D( PackedAttr ),
        D( LastAttr ),
        D( PreprocessingDirective ),
        D( MacroDefinition ),
        D( MacroExpansion ),
        D( MacroInstantiation ),
        D( InclusionDirective ),
        D( FirstPreprocessing ),
        D( LastPreprocessing ),
        D( ModuleImportDecl ),
        D( FirstExtraDecl ),
        D( LastExtraDecl )
#undef D
    };

    /// Default initializes a Cursor object
    constexpr Cursor() = default;

    /// Intializes a CXCursor object from the give CXCursor
    constexpr Cursor( CXCursor cursor ) : M_handle( cursor ) {}

    /// Returns the underlying CXCursor object
    constexpr CXCursor handle() const { return M_handle; }

    /// Returns the cursor's spelling. Don't confuse it with displayName()!
    /// @return std::string of the cursor's spelling
    string spelling() const
    {
        return make_clang_string( clang_getCursorSpelling, handle() );
    }

    /// Returns the cursor's display name. Don't confuse it with spelling()!
    /// @return std::string of the cursor's display name
    string displayName() const
    {
        return make_clang_string( clang_getCursorDisplayName, handle() );
    }

    /// Returns the CXCursorKind value for the underlying CXCursor
    CXCursorKind kind_cx() const { return clang_getCursorKind( handle() ); }
    /// Returns the CXCursorKind, but as a Cursor::Kind value
    Kind kind() const { return Kind( kind_cx() ); }

    /// Compares two cursors for equality using `clang_equalCursors`
    bool operator==( const Cursor& other ) const
    {
         return clang_equalCursors( handle(), other.handle() ) != 0;
    }

    /// Compares two cursors, simply inverting `clang_equalCursors`
    bool operator!=( const Cursor& other ) const { return !( *this == other ); }

    /// Returns true if the cursor is a null cursor
    bool isNull() const { return clang_Cursor_isNull( handle() ) != 0; }

#define DECL_IS_KIND( kind_check )                                             \
    bool is##kind_check() const { return clang_is##kind_check( kind_cx() ) != 0; }

    DECL_IS_KIND( Declaration )
    DECL_IS_KIND( Reference )
    DECL_IS_KIND( Expression )
    DECL_IS_KIND( Statement )
    DECL_IS_KIND( Attribute )
    DECL_IS_KIND( Invalid )
    DECL_IS_KIND( TranslationUnit )
    DECL_IS_KIND( Preprocessing )
    DECL_IS_KIND( Unexposed )

#undef DECL_IS_KIND

    /// Represents a cursor's linkage kind.
    enum Linkage
    {
        /// Inalid linkage
        Invalid = CXLinkage_Invalid,
        /// The cursor has no linkage
        NoLinkage = CXLinkage_NoLinkage,
        /// The cursor has internal linkage
        Internal = CXLinkage_Internal,
        /// The cursor has unique external linkage
        UniqueExternal = CXLinkage_UniqueExternal,
        /// The cursor has plain external linkage
        External = CXLinkage_External
    };

    /// Returns the Linkage for the cursor
    /// @return Linkage specifying the cursor's linkage
    Linkage linkage() const
    {
        return Linkage( clang_getCursorLinkage( handle() ) );
    }

    /// Get the cursor's source language,
    /// @return SourceType with the cursors source language
    SourceType language() const
    {
        return SourceType( clang_getCursorLanguage( handle() ) );
    }

    /// Returns a non-owning copy of the TranslationUnit that this Cursor
    /// belongs to
    /// @return A non-owning clangxx::TranslationUnit for the cursor's
    /// translation unit
    template <typename ResType = clangxx::TranslationUnit>
    ResType translationUnit() const
    {
        return ResType::createNonOwning(
            clang_Cursor_getTranslationUnit( handle() ) );
    }

    /// The semantic availability of a given cursor, determining if its usage
    /// will invoke a warning or an error.
    enum Availablility
    {
        /// The cursor is available
        Available = CXAvailability_Available,
        /// The cursor is deprecated
        Deprecated = CXAvailability_Deprecated,
        /// The cursor is not available, and use will be an error
        NotAvailable = CXAvailability_NotAvailable,
        /// The cursor is available, but not accessible. Usage is an error
        NotAccessible = CXAvailability_NotAccessible
    };

    /// Get the Availability of this cursor
    /// @return Availability specifying the cursor's availability
    Availablility availability() const
    {
        return Availablility( clang_getCursorAvailability( handle() ) );
    }

    /// Returns the semantic parent of the cursor. Don't get it confused
    /// with the lexicalParent()
    /// @return A Cursor which represents the semantic parent of this cursor
    Cursor semanticParent() const
    {
        return clang_getCursorSemanticParent( handle() );
    }

    /// Returns the lexical parent of the cursor. Don't get it confused
    /// with the semanticParent()
    /// @return A Cursor which represents the lexical parent of this cursor
    Cursor lexicalParent() const
    {
        return clang_getCursorLexicalParent( handle() );
    }

    /// Get the Location of the cursor in the source file
    /// @return A Location object representing the cursor's location
    Location location() const
    {
        return clang_getCursorLocation( handle() );
    }

    /// Get the range that wraps the definition of this cursor
    /// @return A SourceRange around the cursor
    SourceRange extent() const
    {
        return clang_getCursorExtent( handle() );
    }

    /// If applicable, get the cursor's definition
    /// @return A new Cursor representing the cursor's definition
    Cursor definition() const
    {
        return clang_getCursorDefinition( handle() );
    }

    /// Get the Cursor that this cursor references, if applicable
    /// @return A new Cursor representing the referenced source item
    Cursor referenced() const
    {
        return clang_getCursorReferenced( handle() );
    }

    /// Get the canonical cursor for this cursor
    /// @return A new Cursor representing the canonical cursor for this one
    Cursor canonical() const
    {
        return clang_getCanonicalCursor( handle() );
    }

    /// Get the USR for this cursor (a unique string identifying the cursor)
    /// @return std::string that is unique for each cursor that compares equal
    std::string USR() const
    {
        return make_clang_string( clang_getCursorUSR, handle() );
    }

    /// Returns a generator for the cursor's children
    /// @return A CursorGenerator with the cursor's children inside
    inline CursorGenerator children() const;

    /// Returns a hash for this cursor
    /// @return An unsigned hash of the cursor
    unsigned hash() const { return clang_hashCursor( handle() ); }

private:
    /// The CXCursor that the Cursor represents. Default initialized to a null
    /// cursor object
    CXCursor M_handle = clang_getNullCursor();
};


/// Models the children of a cursor object. Is fully RandomAccessIterable
class CursorGenerator
{
public:
    using iterator = std::vector<Cursor>::const_iterator;
    using const_iterator = std::vector<Cursor>::const_iterator;
    /// Initializes the generator to represent the children of the given cursor.
    explicit CursorGenerator( const Cursor& owner_ ) : owner{ owner_ }
    {
        auto fn = []( CXCursor c, CXCursor, void* self_ )
        {
            auto self = static_cast<CursorGenerator*>( self_ );
            self->children.emplace_back( c );
            return CXChildVisit_Continue;
        };

        clang_visitChildren( owner.handle(), fn, this );
    }

    /// Returns an iterator to the first child cursor
    const_iterator begin() const { return children.begin(); }
    /// Returns an iterator past the last child cursor
    const_iterator end() const { return children.end(); }
    /// Returns a const_iterator to the first child cursor
    const_iterator cbegin() const { return children.cbegin(); }
    /// Returns a const_iterator past the last child cursor
    const_iterator cend() const { return children.cend(); }

    /// Returns the number of children/cursor in the generator
    std::size_t size() const { return children.size(); }

    /// The Cursor from which the generator was created
    Cursor owner;
    /// A vector of the children of the owning cursor
    std::vector<Cursor> children;
};

inline CursorGenerator Cursor::children() const
{
    return CursorGenerator{ *this };
}
}

#endif // LIBCLANGXX_CURSOR_HPP
