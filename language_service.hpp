#ifndef LANGUAGE_SERVICE_HPP_INCLUDED
#define LANGUAGE_SERVICE_HPP_INCLUDED

#include <sstream>

#include <json.hpp>

#include "./initialize_params.hpp"

#include <libclangxx/index.hpp>

#include <boost/thread/future.hpp>

namespace cls {

using nlohmann::json;

struct ShowMessageRequestParams {
    int type;
    std::string message;
};

class LanguageService {
    struct ErasedServer {
        virtual ~ErasedServer() = default;
        virtual boost::future<json> send_request(std::string method, json params) = 0;
        virtual void send_notification(std::string name, json params) = 0;
    };

    template <typename Server> struct ErasedServerImpl : ErasedServer {
        Server& server;
        boost::future<json> send_request(std::string method, json params) override {
            return server.send_request(method, params);
        }
        void send_notification(std::string method, json params) override {
            server.send_notification(method, params);
        }
        ErasedServerImpl(Server& s)
            : server(s) {}
    };

    std::unique_ptr<ErasedServer> _server;

    void _build_string(std::stringstream&) const {}

    template <typename T, typename... Args>
    void _build_string(std::stringstream& strm, const T& t, const Args&... args) const {
        strm << t;
        _build_string(strm, args...);
    }

    template <typename... Args>
    void _show_message(const Args&... args) const {
        if (_server) {
            std::stringstream strm;
            _build_string(strm, args...);
            ShowMessageRequestParams req;
            req.type = 0;
            req.message = strm.str();
            _server->send_notification("window/showMessage", to_json(req));
        }
    }

    template <typename... Args>
    void _log_message(const Args&... args) const {
        if (_server) {
            std::stringstream strm;
            _build_string(strm, "[clang-languageservice] ", args...);
            _server->send_notification("window/logMessage",
                                       json{ { "type", 0 }, { "message", strm.str() } });
        }
    }

public:
    template <typename ServerType>
    explicit LanguageService(ServerType& server)
        : _server(new ErasedServerImpl<std::decay_t<ServerType>>(server)) {}
    LanguageService(const LanguageService&) = delete;
    LanguageService& operator=(const LanguageService&) = delete;

    langsrv::InitializeResult initialize(langsrv::InitializeParams params) {
        langsrv::InitializeResult ret;
        auto comp = langsrv::CompletionOptions{};
        comp.resolveProvider = true;
        comp.triggerChars = { ":", ".", ">" };
        ret.capabilities.completionProvider = comp;
        ret.capabilities.referencesProvider = true;
        ret.capabilities.definitionProvider = true;
        ret.capabilities.workspaceSymbolProvider = true;
        _log_message("Initialized clang language server");
        return ret;
    }

    void shutdown() {}

    void unknown_message(std::string method, json params) const {
        _log_message("Got an unknown message with method '", method, "'");
    }

    void didOpenTextDocument(const langsrv::DidOpenTextDocumentParams&);
    boost::optional<boost::future<json>> dispatchMethod(std::string method, json params);
    boost::optional<boost::future<json>> _dispatchMethod(std::string method, json params);
};
}

MIRRORPP_REFLECT(cls::ShowMessageRequestParams, (type)(message));

#endif  // LANGUAGE_SERVICE_HPP_INCLUDED