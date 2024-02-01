#include "extensions/asio.hpp"
#include "network_generated.h"

#include <asio/awaitable.hpp>
#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>
#include <asio/use_awaitable.hpp>
#include <asio/write.hpp>
#include <charconv>
#include <concepts>
#include <csignal>
#include <exception>
#include <extensions/std.hpp>
#include <format>
#include <iostream>
#include <ranges>
#include <span>
#include <string_view>

namespace
{
using asio::awaitable;
using asio::detached;
using asio::use_awaitable;
using asio::ip::tcp;
namespace this_coro = asio::this_coro;

void createResponse(flatbuffers::FlatBufferBuilder &fbb, const ::flatbuffers::Vector<uint8_t> *data)
{
  auto content = fbb.CreateVector(data->data(), data->size());
  auto request = echo::CreateRequest(fbb, content);
  fbb.Finish(request);
}

awaitable<void> echo_once(tcp::socket &socket)
{
  std::array<char, 128> data;

  co_await socket.async_read_some(asio::buffer(data), use_awaitable);

  auto request = flatbuffers::GetRoot<echo::Request>(data.data());

  auto message = request->msg_nested_root();

  std::cout << std::format("{} got({}): {}\n", socket.remote_endpoint(), message->id(), message->str()->string_view());

  flatbuffers::FlatBufferBuilder builder;
  createResponse(builder, request->msg());

  co_await async_write(socket, asio::buffer(builder.GetBufferPointer(), builder.GetSize()), use_awaitable);
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

awaitable<void> listener(const asio::ip::tcp::endpoint &endpoint)
{
  auto          executor = co_await this_coro::executor;
  tcp::acceptor acceptor(executor, endpoint);
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

  auto args = std::span(argv, argc) | std::views::transform(static_cast_v<std::string_view>);

  if (args.size() != 3)
  {
    std::cout << std::format("usage: echo_server <host> <port>\n");
  }

  try
  {
    asio::io_context io_context(1);
    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { io_context.stop(); });

    asio::ip::tcp::resolver resolver(io_context);
    co_spawn(io_context, listener(*resolver.resolve(args[1], args[2])), detached);

    io_context.run();

    std::cout << "stop.\n";
  }
  catch (const std::exception &e)
  {
    std::cout << std::format("exception: {}\n", e);
  }

  return 0;
}
