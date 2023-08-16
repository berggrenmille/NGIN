#pragma once
#include <utility>
#include <tuple>
#include <type_traits>
namespace NGIN
{
	namespace Util
	{
		namespace
		{
			/**
			 * @brief Implementation of Apply()
			 * @param t The tuple to be unpacked.
			 * @param f The function to be applied to the tuple's elements.
			 * @param index_sequence A std::index_sequence to access tuple's elements.
			 * @tparam Tuple The tuple type.
			 * @tparam F The function type.
			 * @tparam I The tuple indices.
			 */
			template <class Tuple, class F, std::size_t... I>
			constexpr void Apply_impl(Tuple&& t, F&& f, std::index_sequence<I...>)
			{
				std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))...);
			}

		}

		/**
		 * @brief Helper function to unpack a tuple and apply a function to its elements.
		 * @param t The tuple to be unpacked.
		 * @param f The function to be applied to the tuple's elements.
		 * @param index_sequence A std::index_sequence to access tuple's elements.
		 * @tparam Tuple The tuple type.
		 * @tparam F The function type.
		 * @tparam I The tuple indices.
		 */
		template <class Tuple, class F>
		constexpr void Apply(Tuple&& t, F&& f)
		{
			Apply_impl(std::forward<Tuple>(t), std::forward<F>(f),
					   std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
		}

		/**
		* @brief Helper function to convert a format string and variadic arguments to a tuple.
		* @param first The format string.
		* @param args The variadic arguments.
		* @tparam Args The variadic arguments types.
		* @return A tuple containing the format string and the variadic arguments.
		*/
		template <class... Args>
		std::tuple<const char*, Args...> FORMAT_TO_TUPLE(const char* first, Args&&... args)
		{
			return std::tuple<const char*, Args...>(first, args...);
		}

		template <class... Args>
		std::tuple<Args...> MultArgs(Args&&... args)
		{
			return std::tuple<Args...>(args...);
		}

	}
}
