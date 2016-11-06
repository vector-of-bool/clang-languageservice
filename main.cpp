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

int main(int, char**) {
    // auto server = clang_languageservice::server::create<stdio_iosystem>();
    // server.run();

    json_rpc::server<json_rpc::httpish_transport<json_rpc::stdio_transport>> server;

    using nlohmann::json;
    using std::string;
    cls::LanguageService service(server);
    server.run([&service, &server](std::string method, nlohmann::json params) {
        return service.dispatchMethod(method, params);
    });
    return 0;
}
