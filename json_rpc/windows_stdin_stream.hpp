#ifndef CLS_JSON_RPC_WINDOWS_STDIN_STREAM_HPP_INCLUDED
#define CLS_JSON_RPC_WINDOWS_STDIN_STREAM_HPP_INCLUDED


#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <windows.h>
#include <iostream>
#include <mutex>
#include <thread>
#include <future>

namespace json_rpc
{

namespace asio = boost::asio;

namespace detail
{

class windows_stdin_stream_impl;

class windows_stdin_service : public asio::io_service::service
{
    asio::io_service _ios;
    std::string _pipe_name;
    asio::windows::stream_handle _server_pipe;
    std::promise<void> _done;
    std::atomic_bool _stopped;

    static HANDLE _create_server_pipe(std::string pipe_name)
    {
        const auto server_end = ::CreateNamedPipe(pipe_name.data(),                            // Pipe name
                                                  PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED, // Pipe flags
                                                  PIPE_TYPE_BYTE | PIPE_REJECT_REMOTE_CLIENTS, // We send bytes
                                                  1,                                           // Only one instance
                                                  1024,                                        // 1kb buffer
                                                  1024,                                        // 1kb buffer
                                                  1000,                                        // Default timeout
                                                  nullptr                                      // No security attributes
                                                  );
        assert(server_end != INVALID_HANDLE_VALUE);
        return server_end;
    }

    void _start_stdin_wait()
    {
        std::thread([work=asio::io_service::work(_ios), this] {
            _done.set_value_at_thread_exit();
            ::ConnectNamedPipe(_server_pipe.native_handle(), nullptr);

            DWORD old_console_flags;
            auto input = ::GetStdHandle(STD_INPUT_HANDLE);
            ::GetConsoleMode(input, &old_console_flags);
            const auto console_flags = old_console_flags ^ ENABLE_MOUSE_INPUT ^ ENABLE_WINDOW_INPUT;
            ::SetConsoleMode(input, console_flags);
            char buffer[256];
            const DWORD size = sizeof buffer;
            while (1)
            {
                DWORD nread = 0;
                if (::WaitForSingleObject(input, 100) != WAIT_OBJECT_0)
                {
                    if (_stopped)
                        break;
                }
                const auto did_read = ::ReadFile(input, buffer, size, &nread, nullptr);
                if (!did_read)
                {
                    auto err = boost::system::error_code(::GetLastError(), boost::system::system_category());
                    if (err == boost::system::errc::broken_pipe || err.value() == ERROR_BROKEN_PIPE)
                        break;
                    std::cerr << ::GetLastError() << ": " << std::error_code(::GetLastError(), std::system_category()).message() << std::endl;
                    throw std::system_error{std::error_code(::GetLastError(), std::system_category()), "Ouch"};
                }
                if (nread == 0)
                    break;
                auto buf_ptr = buffer;
                auto remaining = nread;
                while (remaining)
                {
                    try
                    {
                        auto nwritten = _server_pipe.write_some(asio::buffer(buf_ptr, remaining));
                        buf_ptr += nwritten;
                        remaining -= static_cast<DWORD>(nwritten);
                    }
                    catch(boost::system::system_error& e)
                    {
                        if (e.code() == boost::system::errc::broken_pipe)
                        {
                            // Expectable
                            break;
                        }
                    }
                }
            }
            ::SetConsoleMode(input, old_console_flags);
            // std::cerr << "Reached end of stdin" << std::endl;
            ::DisconnectNamedPipe(_server_pipe.native_handle());
            ::CloseHandle(_server_pipe.native_handle());
        }).detach();
        std::thread([this]{ _ios.run(); }).detach();
    }
public:
    using implementation_type = std::shared_ptr<detail::windows_stdin_stream_impl>;
    static asio::io_service::id id;
    explicit windows_stdin_service(asio::io_service& ios)
        : service(ios)
        , _pipe_name("\\\\.\\pipe\\" + to_string(boost::uuids::random_generator()()))
        , _server_pipe(_ios, _create_server_pipe(pipe_name()))
    {
        _stopped.store(false);
        std::cout << "Starting stdin service" << std::endl;
        _start_stdin_wait();
    }

    inline void construct(implementation_type&);
    inline void destroy(implementation_type&);
    void shutdown_service()
    {
        _stopped.store(true);
        _done.get_future().get();
    }

    const std::string& pipe_name() const { return _pipe_name; }
};

class windows_stdin_stream_impl
{
private:
    asio::windows::stream_handle _input_stream;
    asio::streambuf _input_buffer;

    static HANDLE _open_pipe(std::string pipe_name)
    {
        return ::CreateFile(pipe_name.data(),     // The pipe name we just generated,
                            GENERIC_READ,         // Read and write access
                            0,                    // Exclude others,
                            nullptr,              // No security attrs
                            OPEN_EXISTING,        // It's an existing pipe
                            FILE_FLAG_OVERLAPPED, // For async
                            nullptr               // No template
                            );
    }

public:
    windows_stdin_stream_impl(windows_stdin_service& service)
        : _input_stream(service.get_io_service(), _open_pipe(service.pipe_name()))
    {}

    ~windows_stdin_stream_impl()
    {
        _input_stream.close();
    }

    template <typename... Args>
    void async_read_some(Args&&... args)
    {
        _input_stream.async_read_some(std::forward<Args>(args)...);
    }
};

void windows_stdin_service::construct(implementation_type& ptr)
{
    ptr.reset(new windows_stdin_stream_impl(*this));
}

void windows_stdin_service::destroy(implementation_type& ptr)
{
    ptr.reset();
}

}

class windows_stdin_stream : public asio::basic_io_object<detail::windows_stdin_service>
{
public:
    explicit windows_stdin_stream(asio::io_service& ios) : basic_io_object(ios) {}

    template <typename... Args>
    void async_read_some(Args&&... args)
    {
        get_implementation()->async_read_some(std::forward<Args>(args)...);
    }
};
}


#endif // CLS_JSON_RPC_WINDOWS_STDIN_STREAM_HPP_INCLUDED