#include <asio/awaitable.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>
#include <asio/use_awaitable.hpp>
#include <asio/write.hpp>
#include <concepts>
#include <csignal>
#include <exception>
#include <format>
#include <iostream>
#include <string_view>

template<typename T>
concept has_to_string = requires(T v) {
  {
    v.to_string()
  } -> std::convertible_to<std::string_view>;
};

namespace std
{
template<has_to_string T>
std::string_view to_string(const T &v)
{
  return v.to_string();
}
} // namespace std

template<has_to_string T>
struct std::formatter<T>
{
  std::formatter<std::string_view> underlying;

  constexpr auto parse(std::format_parse_context &parse_ctx) { return underlying.parse(parse_ctx); }

  auto format(const T &obj, std::format_context &format_ctx) const
  {
    return underlying.format(std::to_string(obj), format_ctx);
  }
};

template<std::derived_from<std::exception> Exception>
struct std::formatter<Exception>
{
  std::formatter<std::string_view> underlying;

  constexpr auto parse(std::format_parse_context &parse_ctx) { return underlying.parse(parse_ctx); }

  auto format(const Exception &e, std::format_context &format_ctx) const
  {
    return underlying.format(e.what(), format_ctx);
  }
};

template<typename InternetProtocol>
struct std::formatter<asio::ip::basic_endpoint<InternetProtocol>>
{
  std::formatter<std::string_view> underlying;

  constexpr auto parse(std::format_parse_context &parse_ctx) { return underlying.parse(parse_ctx); }

  auto format(const asio::ip::basic_endpoint<InternetProtocol> &endpoint, std::format_context &format_ctx) const
  {
    return underlying.format(
        std::format("{}{{{}, {}}}", endpoint.protocol(), endpoint.address(), endpoint.port()),
        format_ctx
    );
  }
};

template<>
struct std::formatter<asio::ip::tcp>
{
  std::formatter<std::string_view> underlying;

  constexpr auto parse(std::format_parse_context &parse_ctx) { return underlying.parse(parse_ctx); }

  auto format(const asio::ip::tcp &, std::format_context &format_ctx) const
  {
    return underlying.format("tcp", format_ctx);
  }
};

namespace
{
using asio::awaitable;
using asio::detached;
using asio::use_awaitable;
using asio::ip::tcp;
namespace this_coro = asio::this_coro;

awaitable<void> echo_once(tcp::socket &socket)
{
  std::array<char, 128> data;

  auto n = co_await socket.async_read_some(asio::buffer(data), use_awaitable);
  std::cout << std::format("got {}: {}\n", socket.remote_endpoint(), std::string_view(data.data(), n));
  co_await async_write(socket, asio::buffer(data, n), use_awaitable);
}

awaitable<void> echo(tcp::socket socket)
{
  try
  {
    for (;;)
    {
      co_await echo_once(socket);
    }
  }
  catch (const std::system_error &e)
  {
    if (e.code() != asio::error::eof)
    {
      std::cout << std::format("exception: {}\n", e);
    }
  }
  catch (const std::exception &e)
  {
    std::cout << std::format("exception: {}\n", e);
  }
}

awaitable<void> listener(asio::ip::port_type port)
{
  auto          executor = co_await this_coro::executor;
  tcp::acceptor acceptor(executor, {tcp::v6(), port});
  std::cout << std::format("listen: {}\n", acceptor.local_endpoint());
  for (;;)
  {
    tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
    std::cout << std::format("accept: {}\n", socket.remote_endpoint());
    co_spawn(executor, echo(std::move(socket)), detached);
  }
}

} // namespace

int main(int argc, char *argv[])
{
  using asio::detached;
  using asio::ip::port_type;

  if (argc != 2)
  {
    std::cout << std::format("usage: echo_server <port>\n");
  }
  auto port = port_type(std::atoi(argv[1]));

  try
  {
    asio::io_context io_context(1);
    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { io_context.stop(); });

    co_spawn(io_context, listener(port), detached);

    io_context.run();

    std::cout << "stop.\n";
  }
  catch (const std::exception &e)
  {
    std::cout << std::format("exception: {}\n", e);
  }

  return 0;
}
