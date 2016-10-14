#ifndef CLS_JSON_RPC_STREAM_TRANSPORT_HPP_INCLUDED
#define CLS_JSON_RPC_STREAM_TRANSPORT_HPP_INCLUDED

#include <functional>
#include <string>

#include <boost/thread/future.hpp>

#include <json.hpp>

namespace json_rpc
{

using nlohmann::json;
using boost::future;
using boost::system::error_code;
namespace asio = boost::asio;

template <typename InputStream, typename OutputStream = InputStream>
class stream_transport
{
public:
    using in_stream_type = InputStream;
    using out_stream_type = OutputStream;

private:
    in_stream_type& _input_stream;
    out_stream_type& _output_stream;

public:
    stream_transport() = delete;
    stream_transport(in_stream_type& stream)
        : _input_stream(stream)
        , _output_stream(stream)
    {
    }

    stream_transport(in_stream_type& istream,
                     out_stream_type& ostream)
        : _input_stream(istream)
        , _output_stream(ostream)
    {
    }

    bool read(std::ostream& buf)
    {
        auto in = std::istreambuf_iterator<char>(_input_stream);
        auto out = std::ostreambuf_iterator<char>(buf);
        if (!_input_stream.rdbuf()->in_avail())
        {
            if (!_input_stream)
                return false;
            *out = *in;
        }
        const auto avail = _input_stream.rdbuf()->in_avail();
        std::copy_n(in, avail, out);
        return true;
    }

    void write(std::string str)
    {
        _output_stream.write(str.data(), str.size());
    }

    void run()
    {
        _input_stream.get_io_service().run();
    }
};

}

#endif // CLS_JSON_RPC_STREAM_TRANSPORT_HPP_INCLUDED