#include "language_service.hpp"

#include "types.hpp"

#include "opt_bind.hpp"

#include <mirror/mirror.hpp>

#include <algorithm>

using namespace cls;
using namespace langsrv;
using namespace langsrv::optional_bind_op;

using nlohmann::json;
using std::string;

using boost::make_ready_future;
using boost::none;

void LanguageService::didOpenTextDocument(const langsrv::DidOpenTextDocumentParams& p) {
    langsrv::TextDocumentItem doc = p.textDocument;
    getCompilationInfo(GetCompilationInfoParams{ doc.uri })
        .then([=](future<GetCompilationInfoResult> fci) {
            auto res = fci.get();
            res.compilationInfo | [=](CompilationInfo info) {
            };
        })
        .then([this](future<void> f) {
            try {
                f.get();
            } catch (const std::exception& e) {
                _log_message("There was an unhandle error: ", e.what());
            }
        });
}

future<GetCompilationInfoResult>
LanguageService::getCompilationInfo(GetCompilationInfoParams param) {
    return _sendRequest<GetCompilationInfoResult>("vob/cls/getCompilationInfo", param);
}

future<GetCompilationDatabasePathResult>
LanguageService::getCompilationDatabasePath() {
    return _sendRequest<GetCompilationDatabasePathResult>("vob/cls/getCompilationDatabasePath", 0);
}

InitializeResult LanguageService::initialize(const InitializeParams& params) {
    langsrv::InitializeResult ret;
    auto comp = langsrv::CompletionOptions{};
    comp.resolveProvider = true;
    // comp.triggerChars = { ":", ".", ">" };
    // ret.capabilities.completionProvider = comp;
    // ret.capabilities.referencesProvider = true;
    // ret.capabilities.definitionProvider = true;
    // ret.capabilities.workspaceSymbolProvider = true;
    ret.capabilities.renameProvider = true;
    _log_message("Initialized clang language server with ", to_json(ret));
    return ret;
}

boost::optional<boost::future<json>> LanguageService::_dispatchMethod(string method, json params) {
    _log_message("Got request ", method);
    if (method == "initialize") {
        auto res = initialize(from_json<langsrv::InitializeParams>(params));
        auto rj = to_json(res);
        _show_message(MessageType::Info, "Hello, from clang-languageservice!");
        return boost::make_ready_future(rj);
    } else if (method == "textDocument/didOpen") {
        didOpenTextDocument(from_json<langsrv::DidOpenTextDocumentParams>(params));
        return none;
    } else if (method == "textDocument/rename") {
        return json_rpc::convert_result(rename(from_json<langsrv::RenameParams>(params)));
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
        return _dispatchMethod(method, params) | [this](future<json> f) {
            return f.then([this](future<json> f) {
                try {
                    return f.get();
                } catch (const std::exception& e) {
                    _log_message("There was an uncaught exception in the language service: ", e.what());
                    return json();
                }
            });
        };
    } catch (const std::exception& e) {
        _log_message("There was an uncaught exception in the language service: ", e.what());
        return none;
    }
}