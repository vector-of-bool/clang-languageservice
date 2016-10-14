#ifndef CLS_SERVER_HPP_INCLUDED
#define CLS_SERVER_HPP_INCLUDED

#include <functional>
#include <memory>

#include <boost/thread/future.hpp>

#include "json_rpc.hpp"

namespace clang_languageservice
{

class server
{
    struct io_system_base
    {
        virtual ~io_system_base() = default;
        virtual void set_message_handler(json_rpc::method_handler) = 0;
        virtual void run() = 0;
    };

    template <typename T>
    struct io_system : io_system_base
    {
        std::shared_ptr<T> impl;
        explicit io_system(std::shared_ptr<T> impl) : impl(impl) {}
      public:
        template <typename... Args>
        static io_system<T> create(Args &&... args)
        {
            return io_system(T::create(std::forward<Args>(args)...));
        }
        void set_message_handler(json_rpc::method_handler handler) override
        {
            assert(impl);
            impl->set_message_handler(handler);
        }
        void run() override
        {
            assert(impl);
            impl->run();
        }
    };

  private:
    std::unique_ptr<io_system_base> _io;

    explicit server(std::unique_ptr<io_system_base> io) : _io(std::move(io)) {}

  public:
    template <typename IO, typename... Args>
    static server create(Args &&... args)
    {
        using io_system_impl = io_system<IO>;
        return server(std::make_unique<io_system_impl>(io_system_impl::create(std::forward<Args>(args)...)));
    }

    void run()
    {
        assert(_io);
        _io->run();
    }
};
}

#endif // CLS_SERVER_HPP_INCLUDED