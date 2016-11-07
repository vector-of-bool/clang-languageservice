#ifndef CLS_TYPES_HPP_GENERATED_INCLUDED
#define CLS_TYPES_HPP_GENERATED_INCLUDED

#include <map>
#include <string>
#include <vector>

#include <boost/optional.hpp>

#include <json.hpp>
#include <mirror/mirror.hpp>

namespace langsrv {
   using std::string;
   using std::vector;
   using std::map;
   using nlohmann::json;
   using boost::optional;
}

namespace langsrv { struct Position {
    int line;
    int character;
}; }

MIRRORPP_REFLECT(langsrv::Position,
                (line)
                (character)
                );

namespace langsrv { struct Range {
    Position start;
    Position end;
}; }

MIRRORPP_REFLECT(langsrv::Range,
                (start)
                (end)
                );

namespace langsrv { struct Location {
    string uri;
    Range range;
}; }

MIRRORPP_REFLECT(langsrv::Location,
                (uri)
                (range)
                );

namespace langsrv { struct Diagnostic {
    Range range;
    optional<int> severity;
    optional<string> code;
    optional<string> source;
    string message;
}; }

MIRRORPP_REFLECT(langsrv::Diagnostic,
                (range)
                (severity)
                (code)
                (source)
                (message)
                );

namespace langsrv { struct Command {
    string title;
    string command;
    vector<string> arguments;
}; }

MIRRORPP_REFLECT(langsrv::Command,
                (title)
                (command)
                (arguments)
                );

namespace langsrv { struct TextEdit {
    Range range;
    string newText;
}; }

MIRRORPP_REFLECT(langsrv::TextEdit,
                (range)
                (newText)
                );

namespace langsrv { struct WorkspaceEdit {
    map<string, vector<TextEdit>> changes;
}; }

MIRRORPP_REFLECT(langsrv::WorkspaceEdit,
                (changes)
                );

namespace langsrv { struct TextDocumentIdentifier {
    string uri;
}; }

MIRRORPP_REFLECT(langsrv::TextDocumentIdentifier,
                (uri)
                );

namespace langsrv { struct TextDocumentItem {
    string uri;
    string languageId;
    int version;
    string text;
}; }

MIRRORPP_REFLECT(langsrv::TextDocumentItem,
                (uri)
                (languageId)
                (version)
                (text)
                );

namespace langsrv { struct VersionedTextDocumentIdentifier {
    string uri;
    int version;
}; }

MIRRORPP_REFLECT(langsrv::VersionedTextDocumentIdentifier,
                (uri)
                (version)
                );

namespace langsrv { struct TextDocumentPositionParams {
    TextDocumentIdentifier textDocument;
    Position position;
}; }

MIRRORPP_REFLECT(langsrv::TextDocumentPositionParams,
                (textDocument)
                (position)
                );

namespace langsrv { struct ClientCapabilities {
    optional<int> nothing;
}; }

MIRRORPP_REFLECT(langsrv::ClientCapabilities,
                (nothing)
                );

namespace langsrv { struct InitializeParams {
    int processId;
    string rootPath;
    optional<json> initializationOptions;
    ClientCapabilities capabilities;
}; }

MIRRORPP_REFLECT(langsrv::InitializeParams,
                (processId)
                (rootPath)
                (initializationOptions)
                (capabilities)
                );

namespace langsrv { struct CompletionOptions {
    bool resolveProvider;
    vector<string> triggerChars;
}; }

MIRRORPP_REFLECT(langsrv::CompletionOptions,
                (resolveProvider)
                (triggerChars)
                );

namespace langsrv { struct DocumentOnTypeFormattingOptions {
    int nothing;
}; }

MIRRORPP_REFLECT(langsrv::DocumentOnTypeFormattingOptions,
                (nothing)
                );

namespace langsrv { struct SignatureHelpOptions {
    vector<string> triggerChars;
}; }

MIRRORPP_REFLECT(langsrv::SignatureHelpOptions,
                (triggerChars)
                );

namespace langsrv { struct ServerCapabilities {
    optional<int> textDocumentSync;
    optional<bool> hoverProvider;
    optional<CompletionOptions> completionProvider;
    optional<SignatureHelpOptions> signatureHelpOptions;
    optional<bool> definitionProvider;
    optional<bool> referencesProvider;
    optional<bool> documentHighlightProvider;
    optional<bool> workspaceSymbolProvider;
    optional<bool> codeActionsProvider;
    optional<bool> codeLensProvider;
    optional<bool> documentFormattingProvider;
    optional<bool> documentRangeFormatProvider;
    optional<DocumentOnTypeFormattingOptions> documentOnTypeFormattingOptions;
    optional<bool> renameProvider;
}; }

MIRRORPP_REFLECT(langsrv::ServerCapabilities,
                (textDocumentSync)
                (hoverProvider)
                (completionProvider)
                (signatureHelpOptions)
                (definitionProvider)
                (referencesProvider)
                (documentHighlightProvider)
                (workspaceSymbolProvider)
                (codeActionsProvider)
                (codeLensProvider)
                (documentFormattingProvider)
                (documentRangeFormatProvider)
                (documentOnTypeFormattingOptions)
                (renameProvider)
                );

namespace langsrv { struct InitializeResult {
    ServerCapabilities capabilities;
}; }

MIRRORPP_REFLECT(langsrv::InitializeResult,
                (capabilities)
                );

namespace langsrv { struct DidOpenTextDocumentParams {
    TextDocumentItem textDocument;
}; }

MIRRORPP_REFLECT(langsrv::DidOpenTextDocumentParams,
                (textDocument)
                );

namespace langsrv { struct ShowMessageRequestParams {
    int type;
    string message;
}; }

MIRRORPP_REFLECT(langsrv::ShowMessageRequestParams,
                (type)
                (message)
                );

namespace langsrv { struct LogMessageParams {
    int type;
    string message;
}; }

MIRRORPP_REFLECT(langsrv::LogMessageParams,
                (type)
                (message)
                );

namespace cls {
   using std::string;
   using std::vector;
   using std::map;
   using nlohmann::json;
   using boost::optional;
}

namespace cls { struct GetCompilationInfoParams {
    string uri;
}; }

MIRRORPP_REFLECT(cls::GetCompilationInfoParams,
                (uri)
                );

namespace cls { struct CompilationInfo {
    string file;
    string command;
    string directory;
}; }

MIRRORPP_REFLECT(cls::CompilationInfo,
                (file)
                (command)
                (directory)
                );

namespace cls { struct GetCompilationInfoResult {
    optional<CompilationInfo> compilationInfo;
}; }

MIRRORPP_REFLECT(cls::GetCompilationInfoResult,
                (compilationInfo)
                );


#endif