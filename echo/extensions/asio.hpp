#pragma once

#include <asio/ip/tcp.hpp>
#include <format>

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
