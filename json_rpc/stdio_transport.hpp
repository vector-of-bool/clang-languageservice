#ifndef CLS_JSON_RPC_STDIO_TRANSPORT_HPP_INCLUDED
#define CLS_JSON_RPC_STDIO_TRANSPORT_HPP_INCLUDED

#ifdef _WIN32
#include "windows_stdin_stream.hpp"
#else
#include <unistd.h>
#endif

#include "stream_transport.hpp"

#include <ostream>
#include <fstream>


namespace json_rpc
{

static std::ofstream stdin_log{ "cls-stdin.log" };
static std::ofstream stdout_log{ "cls-stdout.log" };

class stdio_transport
{
public:
    stdio_transport() {}

    bool read(std::ostream& out)
    {
        char buffer[256];

        #ifdef _WIN32
        static const auto input = ::GetStdHandle(STD_INPUT_HANDLE);
        DWORD old_console_flags;
        ::GetConsoleMode(input, &old_console_flags);
        const auto console_flags = old_console_flags ^ ENABLE_MOUSE_INPUT ^ ENABLE_WINDOW_INPUT;
        ::SetConsoleMode(input, console_flags);
        DWORD nread = 0;
        ::WaitForSingleObject(input, 100);
        const auto did_read = ::ReadFile(input, buffer, sizeof buffer, &nread, nullptr);
        if (!did_read)
        {
            auto err = std::error_code(::GetLastError(), std::system_category());
            if (err == std::errc::broken_pipe || err.value() == ERROR_BROKEN_PIPE)
                return false;
            std::cerr << err.value() << ": " << err.message() << std::endl;
            throw std::system_error(err, "Ouch");
        }
        ::SetConsoleMode(input, old_console_flags);
        if (nread == 0)
            return false;

        out.write(buffer, nread);
        return true;
        #else
        const auto nread = ::read(STDIN_FILENO, buffer, sizeof buffer);
        #endif

        if (nread == 0)
            return false;

        stdin_log.write(buffer, nread);
        out.write(buffer, nread);
        return true;
    }

    void write(std::string str)
    {
        #ifdef _WIN32
        static const auto output = ::GetStdHandle(STD_OUTPUT_HANDLE);

        auto remaining = str.size();
        auto data = str.data();
        while (remaining) {
            DWORD nwritten = 0;
            ::WriteFile(output, data, static_cast<DWORD>(remaining), &nwritten, nullptr);
            remaining -= nwritten;
            data += nwritten;
        }
        #else
        stdout_log.write(str.data(), str.size());
        stdout_log.flush();
        std::cout.write(str.data(), str.size());
        std::cout.flush();
        #endif
    }
};

}

#endif // CLS_JSON_RPC_STDIO_TRANSPORT_HPP_INCLUDED