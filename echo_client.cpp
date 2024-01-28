#include <asio/awaitable.hpp>
#include <asio/connect.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/write.hpp>
#include <exception>
#include <format>
#include <iostream>

int main(int argc, char *argv[])
{
  using asio::awaitable;
  using asio::ip::tcp;

  enum
  {
    max_length = 1024
  };

  try
  {
    if (argc != 3)
    {
      throw std::runtime_error("Usage: echo-client <host> <port>");
    }

    asio::io_context io_context(1);

    tcp::socket   s(io_context);
    tcp::resolver resolver(io_context);
    connect(s, resolver.resolve(argv[1], argv[2]));

    std::cout << "Enter message: ";
    char request[max_length];
    std::cin.getline(request, max_length);
    auto request_length = std::strlen(request);
    write(s, asio::buffer(request, request_length));

    char reply[max_length];
    auto reply_length = read(s, asio::buffer(reply, request_length));
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";
  }
  catch (const std::exception &e)
  {
    std::cout << std::format("Exception: {}\n", e.what());
  }

  return 0;
}
