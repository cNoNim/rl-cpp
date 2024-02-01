#include "extensions/asio.hpp"
#include "network_generated.h"

#include <array>
#include <asio/awaitable.hpp>
#include <asio/connect.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/readable_pipe.hpp>
#include <asio/write.hpp>
#include <concepts>
#include <exception>
#include <extensions/std.hpp>
#include <format>
#include <iostream>
#include <ranges>
#include <span>
#include <string_view>

namespace
{

void createRequest(flatbuffers::FlatBufferBuilder &fbb, uint32_t id, std::string_view msg)
{
  flatbuffers::FlatBufferBuilder fbb_message;

  auto message = echo::CreateMessage(fbb_message, id, fbb_message.CreateString(msg));
  fbb_message.Finish(message);
  auto content = fbb.CreateVector(fbb_message.GetBufferPointer(), fbb_message.GetSize());
  auto request = echo::CreateRequest(fbb, content);
  fbb.Finish(request);
}

} // namespace

int main(int argc, char *argv[])
{
  using asio::awaitable;
  using asio::ip::tcp;

  auto args = std::span(argv, argc) | std::views::transform(static_cast_v<std::string_view>);

  if (args.size() != 4)
  {
    throw std::runtime_error("usage: echo-client <host> <port> <message>");
  }

  try
  {
    flatbuffers::FlatBufferBuilder builder(128);
    createRequest(builder, 0, args[3]);

    asio::io_context io_context;

    tcp::socket   socket(io_context);
    tcp::resolver resolver(io_context);
    connect(socket, resolver.resolve(args[1], args[2]));
    std::cout << std::format("connect: {}\n", socket.remote_endpoint());

    write(socket, asio::buffer(builder.GetBufferPointer(), builder.GetSize()));

    std::array<char, 128> reply;
    socket.read_some(asio::buffer(reply));
    auto response = flatbuffers::GetRoot<echo::Response>(reply.data());

    auto msg = response->msg_nested_root();
    std::cout << std::format("echo ({}): {}\n", msg->id(), msg->str()->string_view());
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

  return 0;
}
