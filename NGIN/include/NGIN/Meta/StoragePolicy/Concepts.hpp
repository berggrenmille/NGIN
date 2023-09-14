#pragma once
#include <concepts>
#include  <type_traits>
namespace NGIN::Meta::StoragePolicy
{


	template <typename T>
	concept IsStorageWrappable = (std::is_move_assignable_v<T> && std::is_move_constructible_v<T>) || std::is_trivially_copyable_v<T>;

	template <typename T, typename U>
	concept IsNotSame = !std::is_same_v<std::decay_t<T>, std::decay_t<U>>;


}