#pragma once

#include <concepts>
#include <exception>
#include <format>
#include <string_view>

template<typename T>
struct static_cast_t
{
  template<typename S>
  T operator()(S &&source) const
  {
    return static_cast<T>(std::forward<S>(source));
  }
};

template<typename T>
constexpr static auto static_cast_v = static_cast_t<T>();

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
