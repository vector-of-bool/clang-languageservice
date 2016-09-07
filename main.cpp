#include <fstream>
#include <iostream>
#include <string>

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/thread/future.hpp>

#include <json.hpp>

#include <unistd.h>

std::ofstream outfile{"server.log"};

namespace json_rpc
{

using nlohmann::json;
using boost::future;
using boost::system::error_code;
namespace asio = boost::asio;

template <typename InputStream, typename OutputStream = InputStream>
class basic_server
{
public:
    using in_stream_type = InputStream;
    using out_stream_type = OutputStream;
    using handler_type
        = std::function<future<json>(std::string, std::vector<json>)>;

private:
    in_stream_type& _input_stream;
    out_stream_type& _output_stream;
    asio::streambuf _input_buffer;
    handler_type _handler;

public:
    basic_server(in_stream_type& stream, handler_type handler)
        : _input_stream(stream)
        , _output_stream(stream)
        , _handler(handler)
    {
        _start_next_request();
    }

    basic_server(in_stream_type& istream,
                 out_stream_type& ostream,
                 handler_type handler)
        : _input_stream(istream)
        , _output_stream(ostream)
        , _handler(handler)
    {
        _start_next_request();
    }

private:
    void _start_next_request()
    {
        asio::async_read_until(_input_stream,
                               _input_buffer,
                               "\r\n\r\n",
                               std::bind(&basic_server::_parse_headers_1,
                                         this,
                                         std::placeholders::_1,
                                         std::placeholders::_2));
    }
    void _parse_headers_1(error_code ec, std::size_t size)
    {
        outfile << "Got some headers!" << std::endl;
        if (ec)
        {
            outfile << "Error in initial request\n";
            std::terminate();
        }
        std::istream is{&_input_buffer};
        std::string line;
        std::size_t content_length = 0;
        while (true)
        {
            std::getline(is, line);
            boost::trim(line);
            if (line.empty()) break;
            boost::to_lower(line);
            const auto content_length_pos = line.find("content-length:");
            if (content_length_pos == 0)
            {
                const auto colon_pos = line.find(':');
                auto len = line.substr(colon_pos + 1);
                boost::trim(len);
                try
                {
                    content_length = std::stoull(len);
                }
                catch (...)
                {
                    outfile << "Invalid content-length: " << len << "\n";
                    return;  // Abort connection
                }
            }
        }
        if (content_length == 0)
        {
            outfile << "No content-length provided\n";
            return;  // Aborts the connection
        }
        asio::async_read(_input_stream,
                         _input_buffer,
                         [=](error_code ec, std::size_t) {
                             return _input_buffer.size() >= content_length;
                         },
                         std::bind(&basic_server::_parse_body_2,
                                   this,
                                   content_length,
                                   std::placeholders::_1,
                                   std::placeholders::_2));
    }

    void _parse_body_2(std::size_t content_length, error_code ec, std::size_t)
    {
        std::istream is{&_input_buffer};
        std::string buffer;
        buffer.resize(content_length);
        is.read(&buffer[0], buffer.size());
        outfile << "Read buffer data: " << buffer << std::endl;
        auto data = json::parse(buffer);
        auto method = data["method"].operator std::string();
        auto params = data["params"];
        auto fut = _handler(method, params);
        fut.then([=](future<json> response) {
            outfile << "Sending response: " << response.get().dump()
                    << std::endl;
        });
        _start_next_request();
    }
};

using descriptor_server = basic_server<asio::posix::stream_descriptor>;
}

int main(int, char**)
{
    outfile << "Language server has started\n";

    boost::asio::io_service ios;
    boost::asio::posix::stream_descriptor istream{ios, STDIN_FILENO};
    boost::asio::posix::stream_descriptor ostream{ios, STDOUT_FILENO};

    json_rpc::descriptor_server server(istream,
                                       ostream,
                                       [](std::string, json_rpc::json)
                                           -> boost::future<json_rpc::json> {
                                               return boost::make_ready_future(
                                                   json_rpc::json(5));
                                           });
    ios.run();
    outfile << "Stopping language server\n";
}
