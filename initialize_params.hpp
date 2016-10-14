#ifndef INITIALIZE_PARAMS_HPP_INCLUDED
#define INITIALIZE_PARAMS_HPP_INCLUDED

#include <json.hpp>

#include "./json_serialization.hpp"

#include <mirror/mirror.hpp>

namespace langsrv {

using boost::optional;

struct ClientCapabilities {
    boost::optional<int> nothing;
};

struct InitializeParams {
    int processId;
    std::string rootPath;
    optional<nlohmann::json> initializationOptions;
    ClientCapabilities capabilities;
};

struct CompletionOptions {
    bool resolveProvider;
    std::vector<std::string> triggerChars;
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
                 // clang-format on


#endif  // INITIALIZE_PARAMS_HPP_INCLUDED