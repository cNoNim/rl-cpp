#include "extensions/asio.hpp"

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
#include <string_view>

int main(int argc, char *argv[])
{
  using asio::awaitable;
  using asio::ip::tcp;

  if (argc != 4)
  {
    throw std::runtime_error("usage: echo-client <host> <port> <message>");
  }

  try
  {
    asio::io_context io_context;

    tcp::socket   socket(io_context);
    tcp::resolver resolver(io_context);
    connect(socket, resolver.resolve(argv[1], argv[2]));
    std::cout << std::format("connect: {}\n", socket.remote_endpoint());

    auto request = argv[3];
    write(socket, asio::buffer(request, std::strlen(request)));

    std::array<char, 128> reply;

    auto reply_length = socket.read_some(asio::buffer(reply));
    std::cout << std::format("echo: {}\n", std::string_view(reply.data(), reply_length));
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
