#ifndef CLS_JSON_RPC_HTTPISH_TRANSPORT_HPP_INCLUDED
#define CLS_JSON_RPC_HTTPISH_TRANSPORT_HPP_INCLUDED

#include <boost/asio.hpp>

#include <json.hpp>


namespace json_rpc {

namespace asio = boost::asio;
using nlohmann::json;

template <typename StreamTransport> class httpish_transport {
    StreamTransport _streamer;
    using handler_type = std::function<boost::future<json>(json)>;
    handler_type _handler;

public:
    template <typename... Args>
    httpish_transport(Args&&... args)
        : _streamer(std::forward<Args>(args)...) {}

private:
    void _step1_read_headers() {
        std::stringstream stream;
        while (_streamer.read(stream)) {
            auto off = stream.str().find("\r\n\r\n");
            if (off != std::string::npos) {
                const auto do_continue = _step2_parse_headers(off + 4, stream);
                if (!do_continue)
                    return;
            }
        }
    }

    bool _step2_parse_headers(std::size_t headers_size, std::stringstream& data) {
        std::string line;
        std::size_t content_length = 0;
        while (true) {
            std::getline(data, line);
            boost::trim(line);
            if (line.empty())
                break;
            boost::to_lower(line);
            const auto content_length_pos = line.find("content-length:");
            if (content_length_pos == 0) {
                const auto colon_pos = line.find(':');
                auto len = line.substr(colon_pos + 1);
                boost::trim(len);
                try {
                    content_length = static_cast<std::size_t>(std::stoull(len));
                } catch (...) {
                    OUTPUT_STREAM << "Invalid content-length: " << len << "\n";
                    return false;  // Abort connection
                }
            }
        }
        if (content_length == 0) {
            OUTPUT_STREAM << "No content-length provided\n";
            return false;  // Aborts the connection
        }
        data.str(data.str().substr(headers_size));
        return _step3_read_body(content_length, data);
    }

    bool _step3_read_body(std::size_t content_length, std::stringstream& stream) {
        while (stream.str().size() < content_length && _streamer.read(stream)) { /* NOOP */
        }
        std::string buffer;
        buffer.resize(content_length);
        stream.read(&buffer[0], buffer.size());
        stream.str(stream.str().substr(content_length));
        OUTPUT_STREAM << "Got request: " << buffer << std::endl;
        json data;
        try {
            data = json::parse(buffer);
        } catch (std::invalid_argument e) {
            OUTPUT_STREAM << "Invalid json: " << e.what() << std::endl;
            return false;
        }

        _handler(data).then([this](future<json> response) { send_message(response.get()); });
        return true;
    }

public:
    void send_message(json msg) {
        const auto str = msg.dump();
        OUTPUT_STREAM << "Sending message: " << str << std::endl;
        static std::mutex responding_lock;
        std::lock_guard<std::mutex> lk{ responding_lock };
        _streamer.write("Content-Length: ");
        _streamer.write(std::to_string(str.length() + 2));
        _streamer.write("\r\n\r\n");
        _streamer.write(str);
        _streamer.write("\r\n");
    }
    template <typename Handler> void run(Handler&& h) {
        _handler = std::ref(h);
        _step1_read_headers();
        // _streamer.run();
    }
};
}

#endif  // CLS_JSON_RPC_HTTPISH_TRANSPORT_HPP_INCLUDED