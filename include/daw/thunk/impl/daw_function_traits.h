// Copyright (c) Darrell Wright
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/beached/json_rpc
//

#ifndef DAW_THUNK_IMPL_FUNCTION_TRAITS_H
#define DAW_THUNK_IMPL_FUNCTION_TRAITS_H

#include <cstddef>
#include <tuple>
#include <utility>

namespace daw::func {
	template<typename, typename = void>
	struct function_traits;

	// function pointer
	template<typename R, typename... Args>
	struct function_traits<R ( * )( Args... )> : function_traits<R( Args... )> {};

	template<typename R, typename... Args>
	struct function_traits<R( Args... )> {
		using result_t = R;

		static constexpr std::size_t arity = sizeof...( Args );

		template<std::size_t N>
		struct argument {
			static_assert( N < arity, "error: invalid parameter index." );
			using type = std::tuple_element_t<N, std::tuple<Args...>>;
		};
	};

	// member function pointer
	template<typename C, typename R, typename... Args>
	struct function_traits<R ( C::* )( Args... )>
	  : function_traits<R( C &, Args... )> {};

	// const member function pointer
	template<typename C, typename R, typename... Args>
	struct function_traits<R ( C::* )( Args... ) const>
	  : function_traits<R( C &, Args... )> {};

	// member object pointer
	template<typename C, typename R>
	struct function_traits<R( C::* )> : function_traits<R( C & )> {};

	// functor
	template<typename F>
	struct function_traits<F, std::enable_if_t<std::is_class_v<F>>> {
	private:
		using call_type = function_traits<decltype( &F::operator( ) )>;

	public:
		using result_t = typename call_type::result_t;

		static constexpr std::size_t arity = call_type::arity - 1;
		template<std::size_t N>
		struct argument {
			static_assert( N < arity, "error: invalid parameter index." );
			using type = typename call_type::template argument<N + 1>::type;
		};
	};
} // namespace daw::func

#endif /* ifndef DAW_THUNK_IMPL_FUNCTION_TRAITS_H */
