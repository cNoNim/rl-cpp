#include <array>
#include <asio/awaitable.hpp>
#include <asio/connect.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/read.hpp>
#include <asio/readable_pipe.hpp>
#include <asio/write.hpp>
#include <exception>
#include <format>
#include <iostream>

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
