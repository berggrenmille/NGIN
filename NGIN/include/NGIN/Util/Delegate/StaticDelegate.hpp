#pragma once
#include <NGIN/Defines.hpp>
#include <type_traits>
#include <concepts>
#include <NGIN/Meta/StoragePolicy.hpp>

namespace NGIN::Util
{

    template <typename FuncType>
    class StaticDelegate;

    template <typename ReturnType, typename... ArgTypes>
    class StaticDelegate<ReturnType(ArgTypes...)>
    {
    public:
        /// \brief Type of the function pointer.
        using FunctionType = ReturnType (*)(ArgTypes...);

        /// \brief Type of the return value.
        using ResultType = ReturnType;

        /// \brief Number of arguments.
        static constexpr size_t NumArgs = sizeof...(ArgTypes);

        /// \brief Tuple of argument types.
        using ArgsTupleType = std::tuple<ArgTypes...>;

        template <typename F>
            requires std::is_invocable_v<std::decay_t<F>, ArgTypes...>
        StaticDelegate(F &&f) noexcept
        {
            using CallableType = std::decay_t<F>;

            storage = StorageType(CallableType(std::forward<F>(f)));
            invoker = InvokeCallable<CallableType>;
        }

        StaticDelegate(ReturnType (*func)(ArgTypes...)) noexcept
        {

            using FuncType = ReturnType (*)(ArgTypes...);
            storage = StorageType(FuncType(func));

            invoker = InvokeCallable<FuncType>;
        }

        template <class T>
        StaticDelegate(ReturnType (T::*func)(ArgTypes...), T *obj) noexcept
        {
            auto wrapper = [func, obj](ArgTypes... args)
            {
                return (obj->*func)(std::forward<ArgTypes>(args)...);
            };

            using WrapperType = decltype(wrapper);

            storage = StorageType(std::move(wrapper));
            invoker = InvokeCallable<WrapperType>;
        }

        auto operator()(ArgTypes... args) -> decltype(auto)
        {

            return invoker(storage.get(), std::forward<ArgTypes>(args)...);
        }

    private:
        static constexpr size_t BUFFER_SIZE = sizeof(void *) * 4;

        using InvokerFunc = ReturnType (*)(void *, ArgTypes &&...);
        using StorageType = Meta::StoragePolicy::HybridStorage<BUFFER_SIZE>;

        using DestroyerFunc = void (*)(void *);

        StorageType storage;
        InvokerFunc invoker = nullptr;

        template <typename Callable>
        inline static auto InvokeCallable(void *storage, ArgTypes &&...args) -> decltype(auto)
        {
            auto &callable = *static_cast<Callable *>(storage);
            return callable(std::forward<ArgTypes>(args)...);
            //   return InvokeImpl<Callable, Args...>(callable, std::forward<Args>(args)...);
        }
        /*
                template <typename Callable, typename First, typename... Rest>
                inline static auto InvokeImpl(Callable &callable, First &&first, Rest &&...rest) -> decltype(auto)
                {
                    return callable(std::forward<First>(first), std::forward<Rest>(rest)...);
                }

                template <typename Callable>
                inline static auto InvokeImpl(Callable &callable) -> decltype(auto)
                {
                    return callable();
                }*/
    };

}