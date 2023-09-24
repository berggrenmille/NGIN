#pragma once
// FunctionTraits.hpp
#include <tuple>
namespace NGIN::Meta
{
    template <typename F>
    struct FunctionTraits;

    // function pointer
    template <typename R, typename... Args>
    struct FunctionTraits<R(*)(Args...)>
    {
        using ReturnType = R;
        using ArgsTupleType = std::tuple<Args...>;
        static constexpr auto argsIndexSequence = std::make_index_sequence<std::tuple_size_v<ArgsTupleType>>();
    };

    // member function pointer
    template <typename C, typename R, typename... Args>
    struct FunctionTraits<R(C::*)(Args...)>
    {
        using ReturnType = R;
        using ArgsTupleType = std::tuple<Args...>;
        static constexpr auto argsIndexSequence = std::make_index_sequence<std::tuple_size_v<ArgsTupleType>>();
    };

    // const member function pointer
    template <typename C, typename R, typename... Args>
    struct FunctionTraits<R(C::*)(Args...) const>
    {
        using ReturnType = R;
        using ArgsTupleType = std::tuple<Args...>;
        static constexpr auto argsIndexSequence = std::make_index_sequence<std::tuple_size_v<ArgsTupleType>>();
    };

    // callable objects, including lambdas
    template <typename F>
    struct FunctionTraits : FunctionTraits<decltype(&F::operator())>
    {};
}