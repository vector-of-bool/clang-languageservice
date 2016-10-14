#include <fstream>
#include <iostream>
#include <string>
#include <thread>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/thread/future.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>


#define OUTPUT_STREAM outfile

#include <json.hpp>

#include "json_rpc/httpish_transport.hpp"
#include "json_rpc/server.hpp"
#include "json_rpc/stdio_transport.hpp"


#ifndef _WIN32
#include <unistd.h>
#endif

#include "json_rpc/stream_transport.hpp"

#include "./language_service.hpp"
#include "server.hpp"


std::ofstream outfile{ "server.log" };

int main(int, char**) {
    outfile << "Language server has started\n";

    // auto server = clang_languageservice::server::create<stdio_iosystem>();
    // server.run();

    json_rpc::server<json_rpc::httpish_transport<json_rpc::stdio_transport>> server;

    using nlohmann::json;
    using std::string;
    cls::LanguageService service(server);
    server.run([&service, &server](std::string method, nlohmann::json params) {
        if (method == "initialize") {
            auto res = service.initialize(from_json<langsrv::InitializeParams>(params));
            auto rj = to_json(res);

            cls::ShowMessageRequestParams msg;
            server.send_request("window/showMessageRequest", to_json(msg));

            return boost::make_ready_future(rj);
        }
        if (method == "shutdown") {
            service.shutdown();
            return boost::make_ready_future(json{});
        }
        service.unknown_message(method, params);
        return boost::make_ready_future(nlohmann::json());
    });
    outfile << "Stopping language server\n";
    return 0;
}
