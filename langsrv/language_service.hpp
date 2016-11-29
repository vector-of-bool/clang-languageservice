#ifndef LANGUAGE_SERVICE_HPP_INCLUDED
#define LANGUAGE_SERVICE_HPP_INCLUDED

#include "protocol_types.hpp"

#include <json_rpc/serialize.hpp>

#include <json.hpp>

#include <boost/thread/future.hpp>

#include <fstream>
#include <sstream>

namespace cls {

using nlohmann::json;
using boost::future;
using boost::optional;

using json_rpc::to_json;
using json_rpc::from_json;

static std::ofstream cls_log{ "cls-messages.log" };
}

namespace cls {

class LanguageService {
    struct ErasedServer {
        virtual ~ErasedServer() = default;
        virtual future<json> sendRequest(const std::string& method, const json& params) = 0;
        virtual void sendNotification(const std::string& name, const json& params) = 0;
        template <typename T>
        typename std::enable_if<!std::is_same<T, json>::value, future<json>>::type
        sendRequest(const std::string& method, const T& data) {
            return sendRequest(method, to_json(data));
        }
        template <typename T>
        typename std::enable_if<!std::is_same<T, json>::value>::type
        sendNotification(const std::string& method, const T& data) {
            sendNotification(method, to_json(data));
        }
    };

    template <typename Server> struct ErasedServerImpl : ErasedServer {
        Server& server;
        future<json> sendRequest(const std::string& method, const json& params) override {
            return server.send_request(method, params);
        }
        void sendNotification(const std::string& method, const json& params) override {
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
    void _show_message(langsrv::MessageType type, const Args&... args) const {
        if (_server) {
            std::stringstream strm;
            _build_string(strm, args...);
            langsrv::ShowMessageRequestParams req;
            req.type = static_cast<int>(type);
            req.message = strm.str();
            cls_log << strm.str() << std::endl;
            _server->sendNotification("window/showMessage", req);
        }
    }

    template <typename... Args> void _log_message(const Args&... args) const {
        if (_server) {
            std::stringstream strm;
            _build_string(strm, "[clang-languageservice] ", args...);
            cls_log << strm.str() << std::endl;
            std::cerr << strm.str() << std::endl;
            langsrv::LogMessageParams req;
            req.type = 4;
            req.message = strm.str();
            _server->sendNotification("window/logMessage", req);
        }
    }

    template <typename Result, typename Params>
    future<Result> _sendRequest(const std::string& method, const Params& params) {
        assert(_server);
        return _server->sendRequest(method, to_json(params)).then([](future<json> fj) {
            return from_json<Result>(fj.get());
        });
    }

public:
    template <typename ServerType>
    explicit LanguageService(ServerType& server)
        : _server(new ErasedServerImpl<ServerType>(server)) {}
    LanguageService(const LanguageService&) = delete;
    LanguageService& operator=(const LanguageService&) = delete;

    future<GetCompilationInfoResult> getCompilationInfo(GetCompilationInfoParams param);
    future<GetCompilationDatabasePathResult> getCompilationDatabasePath();

    langsrv::InitializeResult initialize(const langsrv::InitializeParams& params);
    future<langsrv::WorkspaceEdit> rename(const langsrv::RenameParams& params);

    void shutdown() {}

    void unknown_message(std::string method, json params) const {
        _log_message("Got an unknown message with method '", method, "'");
    }

    void didOpenTextDocument(const langsrv::DidOpenTextDocumentParams&);
    boost::optional<future<json>> dispatchMethod(std::string method, json params);
    boost::optional<future<json>> _dispatchMethod(std::string method, json params);
};
}


#endif  // LANGUAGE_SERVICE_HPP_INCLUDED