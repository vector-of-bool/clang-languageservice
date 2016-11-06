#ifndef CLS_JSON_RPC_SERVER_HPP_INCLUDED
#define CLS_JSON_RPC_SERVER_HPP_INCLUDED

#include <boost/asio.hpp>
#include <boost/thread/future.hpp>

#include <json.hpp>

namespace json_rpc {

using nlohmann::json;
using boost::future;

namespace asio = boost::asio;

struct transport_init_tag_t {};
namespace {
transport_init_tag_t transport_init;
}

template <typename Transport> class server {
private:
    Transport _transporter;

public:
    template <typename... Args>
    server(Args&&... args)
        : _transporter(std::forward<Args>(args)...) {}

    template <typename Func> int run(Func&& fn) {
        _transporter.run(
            [ this, fn ](const json& data)
                ->boost::optional<future<json>> {
                    if (!data.is_object()) {
                        OUTPUT_STREAM << "Invalid JSON, expected an object or array" << std::endl;
                        return boost::make_exceptional_future<json>(
                            std::invalid_argument("Invalid JSON, expected on object or array"));
                    }
                    auto method_json = data["method"];
                    if (!method_json.is_string()) {
                        OUTPUT_STREAM << "Invalid body: 'method' must be a string\n";
                        return boost::make_exceptional_future<json>(
                            std::invalid_argument("Invalid body: 'method' must be a string."));
                    }
                    auto method = method_json.operator std::string();
                    auto params_iter = data.find("params");
                    auto params = params_iter == end(data) ? json{} : *params_iter;

                    auto maybe_fut = fn(method, params);
                    if (maybe_fut) {
                        return maybe_fut->then([data](future<json> result) -> json {
                            if (result.has_exception())
                                std::terminate();

                            json ret;
                            auto id_iter = data.find("id");
                            if (id_iter != end(data)) {
                                ret["id"] = data["id"];
                            }
                            ret["jsonrpc"] = "2.0";
                            ret["result"] = result.get();
                            return ret;
                        });
                    }
                    return boost::none;
                });
        return 0;
    }

    boost::future<json> send_request(std::string method, json data) {
        json msg;
        msg["id"] = "Meow";
        msg["jsonrpc"] = "2.0";
        msg["method"] = method;
        msg["params"] = data;
        _transporter.send_message(msg);
        return boost::make_ready_future(json{});
    }

    void send_notification(std::string method, json data) {
        json msg;
        msg["jsonrpc"] = "2.0";
        msg["method"] = method;
        msg["params"] = data;
        _transporter.send_message(msg);
    }
};
}

#endif  // CLS_JSON_RPC_SERVER_HPP_INCLUDED