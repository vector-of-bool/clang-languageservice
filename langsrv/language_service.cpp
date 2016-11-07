#include "language_service.hpp"

#include <mirror/mirror.hpp>

using namespace cls;
using namespace langsrv;

using nlohmann::json;
using std::string;

using boost::make_ready_future;
using boost::none;

void LanguageService::didOpenTextDocument(const langsrv::DidOpenTextDocumentParams& p) {
    _log_message("Opened text document ", to_json(p));
}

InitializeResult LanguageService::initialize(const InitializeParams& params) {
    langsrv::InitializeResult ret;
    auto comp = langsrv::CompletionOptions{};
    comp.resolveProvider = true;
    comp.triggerChars = { ":", ".", ">" };
    ret.capabilities.completionProvider = comp;
    ret.capabilities.referencesProvider = true;
    ret.capabilities.definitionProvider = true;
    ret.capabilities.workspaceSymbolProvider = true;
    _log_message("Initialized clang language server with ", to_json(ret));
    return ret;
}

boost::optional<boost::future<json>> LanguageService::_dispatchMethod(string method, json params) {
    _log_message("Got request ", method);
    if (method == "initialize") {
        auto res = initialize(from_json<langsrv::InitializeParams>(params));
        auto rj = to_json(res);
        _show_message("Hello, from clang-languageservice!");
        return boost::make_ready_future(rj);
    } else if (method == "textDocument/didOpen") {
        didOpenTextDocument(from_json<langsrv::DidOpenTextDocumentParams>(params));
        return none;
    } else if (method == "shutdown") {
        shutdown();
        return boost::make_ready_future(json());
    } else {
        unknown_message(method, params);
        return boost::make_ready_future(json());
    }
}

boost::optional<boost::future<json>> LanguageService::dispatchMethod(string method, json params) {
    try {
        return _dispatchMethod(method, params);
    } catch (const std::exception& e) {
        _log_message("There was an uncaught exception in the language service: ", e.what());
        return none;
    }
}