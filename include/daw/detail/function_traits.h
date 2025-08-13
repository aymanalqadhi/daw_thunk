#ifndef DAW_DETAIL_FUNCTION_TRAITS_H
#define DAW_DETAIL_FUNCTION_TRAITS_H

#include "daw/detail/typelist.h"

#include <concepts>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace daw::detail {

template <typename T>
concept Thunkable = std::is_same_v<T, void> || std::is_reference_v<T> ||
                    std::is_trivially_copyable_v<T>;

template <typename T>
struct function_traits;

template <Thunkable R, Thunkable... Args>
struct function_traits<typelist<R, Args...>> {
  using result_t    = R;
  using params_t    = typelist<Args...>;
  using signature_t = R(Args...);

  static constexpr std::size_t arity = params_t::size;

  template <std::size_t I>
  using erase_param = function_traits<
      typename params_t::template erase<I>::template insert<R, 0>>;
};

template <Thunkable R, Thunkable... Args>
struct function_traits<R(Args...)> : function_traits<typelist<R, Args...>> {};

template <Thunkable R, Thunkable... Args>
struct function_traits<R (*)(Args...)> : function_traits<R(Args...)> {};

template <typename C, Thunkable R, Thunkable... Args>
struct function_traits<R (C::*)(Args...)> : function_traits<R(C&, Args...)> {};

template <typename C, Thunkable R, Thunkable... Args>
struct function_traits<R (C::*)(Args...) const>
    : function_traits<R(const C&, Args...)> {};

template <typename Fn>
  requires(std::is_class_v<Fn>)
struct function_traits<Fn> : function_traits<decltype(&Fn::operator())> {};

} // namespace daw::detail

#endif /* ifndef DAW_DETAIL_FUNCTION_TRAITS_H */
