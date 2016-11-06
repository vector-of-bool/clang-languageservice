#ifndef LANGSRV_PROTOCOL_HPP_INCLUDED
#define LANGSRV_PROTOCOL_HPP_INCLUDED

#include <json.hpp>

#include "serialize.hpp"

#include <mirror/mirror.hpp>

namespace langsrv {

using boost::optional;
using std::string;
using std::vector;
using nlohmann::json;

struct ClientCapabilities {
    boost::optional<int> nothing;
};

struct InitializeParams {
    int processId;
    string rootPath;
    optional<json> initializationOptions;
    ClientCapabilities capabilities;
};

struct CompletionOptions {
    bool resolveProvider;
    vector<string> triggerChars;
};

struct SignatureHelpOptions {
    int nothing;
};

struct DocumentOnTypeFormattingOptions {
    int nothing;
};

struct ServerCapabilities {
    optional<int> textDocumentSync;
    optional<bool> hoverProvider;
    optional<CompletionOptions> completionProvider;
    optional<SignatureHelpOptions> signatureHelpOptions;
    optional<bool> definitionProvider;
    optional<bool> referencesProvider;
    optional<bool> documentHighlightProvider;
    optional<bool> workspaceSymbolProvider;
    optional<bool> codeActionProvider;
    optional<bool> codeLensProvider;
    optional<bool> documentFormattingProvider;
    optional<bool> documentRangeFormatProvider;
    optional<DocumentOnTypeFormattingOptions> documentOnTypeFormattingProvider;
    optional<bool> renameProvider;
};

struct InitializeResult {
    langsrv::ServerCapabilities capabilities;
};

struct TextDocumentItem {
    string uri;
    string languageId;
    int version;
    string text;
};


struct DidOpenTextDocumentParams {
    TextDocumentItem textDocument;
};

}

// clang-format off
MIRRORPP_REFLECT(langsrv::ClientCapabilities, (nothing));
MIRRORPP_REFLECT(langsrv::InitializeParams,
                 (processId)(rootPath)(initializationOptions)(capabilities));
MIRRORPP_REFLECT(langsrv::SignatureHelpOptions, (nothing));
MIRRORPP_REFLECT(langsrv::DocumentOnTypeFormattingOptions, (nothing));
MIRRORPP_REFLECT(langsrv::InitializeResult, (capabilities));
MIRRORPP_REFLECT(langsrv::CompletionOptions,
                 (resolveProvider)
                 (triggerChars));
MIRRORPP_REFLECT(langsrv::ServerCapabilities,
                 (textDocumentSync)
                 (hoverProvider)
                 (completionProvider)
                 (signatureHelpOptions)
                 (definitionProvider)
                 (referencesProvider)
                 (documentHighlightProvider)
                 (workspaceSymbolProvider)
                 (codeActionProvider)
                 (codeLensProvider)
                 (documentFormattingProvider)
                 (documentRangeFormatProvider)
                 (documentOnTypeFormattingProvider)
                 (renameProvider));
MIRRORPP_REFLECT(langsrv::TextDocumentItem,
                 (uri)
                 (languageId)
                 (version)
                 (text));
MIRRORPP_REFLECT(langsrv::DidOpenTextDocumentParams,
                 (textDocument));
// clang-format on


#endif // LANGSRV_PROTOCOL_HPP_INCLUDED