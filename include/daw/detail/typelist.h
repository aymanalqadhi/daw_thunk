#ifndef DAW_DETAIL_TYPE_LIST_H
#define DAW_DETAIL_TYPE_LIST_H

#include <any>
#include <concepts>
#include <cstddef>
#include <tuple>
#include <utility>

namespace daw::detail {

template <std::size_t L, std::size_t H>
  requires(L <= H)
consteval auto make_index_range() {
  constexpr auto adjust = []<std::size_t... Is>(std::index_sequence<Is...>) {
    return std::index_sequence<L + Is...>{};
  };

  if constexpr (L == H) {
    return std::index_sequence<>{};
  } else {
    return adjust(std::make_index_sequence<H - L>{});
  }
}

template <std::size_t L, std::size_t H>
using index_range = decltype(make_index_range<L, H>());

template <typename... Ts>
struct typelist {
  static constexpr std::size_t size = sizeof...(Ts);
  static constexpr bool empty       = size == 0;

  template <std::size_t I>
    requires(I < size)
  using nth = std::tuple_element_t<I, std::tuple<Ts...>>;

  template <std::size_t... Is>
  using keep = typelist<nth<Is>...>;

  template <std::size_t... Is>
  static consteval auto select(std::index_sequence<Is...>) -> keep<Is...> {
    return keep<Is...>{};
  }

  template <std::size_t I, std::size_t N = size>
  using sublist = decltype(select(make_index_range<I, I + N>()));

  template <typename T>
  using push_back = typelist<Ts..., T>;

  template <typename T>
  using push_front = typelist<T, Ts...>;

  using pop_back = sublist<0, empty ? 0 : size - 1>;

  using pop_front = sublist<1, empty ? 0 : size - 1>;

  template <typename U, std::size_t I = size>
  using insert =
      decltype(sublist<0, I>{} + typelist<U>{} + sublist<I, size - I>{});

  template <std::size_t I>
  using erase = decltype(sublist<0, I>{} + sublist<I + 1, size - I - 1>{});

  template <template <typename> class W>
  using wrap = typelist<W<Ts>...>;

  template <template <typename> class M>
  using map = typelist<typename M<Ts>::type...>;

  template <typename... Us>
  consteval auto operator+(typelist<Us...>) const noexcept {
    return typelist<Ts..., Us...>{};
  }
};

} // namespace daw::detail

#endif // !DAW_DETAIL_TYPE_LIST_H
