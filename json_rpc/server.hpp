#ifndef CLS_JSON_RPC_SERVER_HPP_INCLUDED
#define CLS_JSON_RPC_SERVER_HPP_INCLUDED

#include <boost/asio.hpp>
#include <boost/thread/future.hpp>

#include <algorithm>
#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <stdexcept>
#include <string>

#include <json.hpp>

namespace json_rpc {

using nlohmann::json;
using boost::future;

namespace asio = boost::asio;

struct transport_init_tag_t {};
namespace {
transport_init_tag_t transport_init;
}

class error : public std::runtime_error {
    int _code = -1;

public:
    error(int code, std::string msg)
        : runtime_error(msg)
        , _code{ code } {}
};

template <typename Transport> class server {
private:
    Transport _transporter;
    std::atomic_int _request_id;
    std::mutex _req_lock;
    std::map<int, std::shared_ptr<boost::promise<json>>> _outstanding_requests;

    void _fulfill_promise(const json& data,
                          json::const_iterator result_iter,
                          json::const_iterator error_iter) {
        std::shared_ptr<boost::promise<json>> prom_ptr;
        {
            std::lock_guard<std::mutex> lk{ _req_lock };
            auto id_iter = data.find("id");
            if (id_iter == end(data)) {
                OUTPUT_STREAM << "Invalid response: 'id' must exist for response/error objects "
                              << std::endl;
                std::terminate();
            }
            auto id = static_cast<int>(*id_iter);
            auto prom = _outstanding_requests.find(id);
            if (prom == end(_outstanding_requests)) {
                OUTPUT_STREAM << "Invalid response: No promise with ID " << id << std::endl;
                std::terminate();
            }
            prom_ptr = prom->second;
            _outstanding_requests.erase(prom);
        }
        assert(prom_ptr);
        if (result_iter != end(data)) {
            prom_ptr->set_value(*result_iter);
        } else if (error_iter != end(data)) {
            auto& error_data = *error_iter;
            auto msg_iter = error_data.find("message");
            auto msg = msg_iter != end(error_data) ? msg_iter->operator std::string()
                                                   : "No message attached to error";
            auto code_iter = error_data.find("code");
            auto code = code_iter != end(error_data) ? int(*code_iter) : -1;
            OUTPUT_STREAM << "Got an error: " << msg << std::endl;
            prom_ptr->set_exception(error{ code, msg });
        } else {
            std::terminate();
        }
    }

public:
    template <typename... Args>
    server(Args&&... args)
        : _transporter(std::forward<Args>(args)...) {
        _request_id = 0;
    }

    template <typename Func> int run(Func&& fn) {
        _transporter.run([this, fn](const json& data) -> boost::optional<future<json>> {
            if (!data.is_object()) {
                OUTPUT_STREAM << "Invalid JSON, expected an object or array" << std::endl;
                return boost::make_exceptional_future<json>(
                    std::invalid_argument("Invalid JSON, expected on object or array"));
            }

            // Check if this message fulfills a request
            const auto result_iter = data.find("result");
            const auto error_iter = data.find("error");
            if (result_iter != end(data) || error_iter != end(data)) {
                _fulfill_promise(data, result_iter, error_iter);
                return boost::none;
            }

            // If not, it must be a request/notification
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
        auto prom = std::make_shared<boost::promise<json>>();
        auto id = _request_id.fetch_add(1);
        auto iter = _outstanding_requests.find(id);
        assert(iter == end(_outstanding_requests));
        _outstanding_requests.insert(std::make_pair(id, prom));
        json msg;
        msg["id"] = id;
        msg["jsonrpc"] = "2.0";
        msg["method"] = method;
        msg["params"] = data;
        _transporter.send_message(msg);
        return prom->get_future();
    }

    void send_notification(std::string method, json data) {
        json msg;
        msg["jsonrpc"] = "2.0";
        msg["method"] = method;
        msg["params"] = data;
        _transporter.send_message(msg);
    }
};

using method_handler = std::function<boost::future<json>(const std::string&, const json&)>;
}

#endif  // CLS_JSON_RPC_SERVER_HPP_INCLUDED