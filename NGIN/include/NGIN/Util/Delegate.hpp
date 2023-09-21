#pragma once
#include <NGIN/Defines.hpp>
#include <type_traits>
#include <NGIN/Meta/StoragePolicy.hpp>
#include <functional>
namespace NGIN::Util
{
    class Delegate
    {

    public:
        Delegate() : storage(), invoker(nullptr), destroyer(nullptr){};

        // Handling Free Functions and Non-capturing Lambdas
        template <typename F>
        Delegate(F &&f) noexcept
        {
            using CallableType = std::decay_t<F>;

            storage = StorageType(CallableType(std::forward<F>(f)));
            invoker = reinterpret_cast<InvokerFunc<void>>(InvokeCallable<CallableType>);
            destroyer = &DestroyCallable<CallableType>;
        }

        template <typename R, typename... Args>
        Delegate(R (*func)(Args...)) noexcept
        {

            using FuncType = R (*)(Args...);
            storage = StorageType(FuncType(func));

            invoker = reinterpret_cast<InvokerFunc<void>>(InvokeCallable<FuncType, Args...>);
            destroyer = &DestroyCallable<FuncType>;
        }

        // Handling Member Functions
        template <class T, typename R, typename... Args>
        Delegate(R (T::*func)(Args...), T *obj) noexcept
        {
            auto wrapper = [func, obj](Args &&...args)
            {
                return (obj->*func)(std::forward<Args>(args)...);
            };

            using WrapperType = decltype(wrapper);

            storage = StorageType(std::move(wrapper));
            invoker = reinterpret_cast<InvokerFunc<void>>(InvokeCallable<WrapperType, Args...>);
            destroyer = &DestroyCallable<WrapperType>;
        }

        ~Delegate()
        {
            if (destroyer)
            {
                destroyer(storage.get());
            }
        }

        template <typename... Args>
        void operator()(Args &&...args)
        {

            reinterpret_cast<InvokerFunc<void, Args...>>(invoker)(storage.get(), std::forward<Args>(args)...);
            //   return invoker(storage.get(), args_arr);
        }

        template <typename R, typename... Args>
        R Invoke(Args &&...args)
        {

            return reinterpret_cast<InvokerFunc<R, Args...>>(invoker)(storage.get(), std::forward<Args>(args)...);
        }

    private:
        static constexpr size_t BUFFER_SIZE = sizeof(void *) * 4;

        using StorageType = Meta::StoragePolicy::HybridStorage<BUFFER_SIZE>;
        template <typename R, typename... Args>
        using InvokerFunc = R (*)(void *, Args &&...);
        using DestroyerFunc = void (*)(void *);

        StorageType storage;
        InvokerFunc<void> invoker = nullptr;
        DestroyerFunc destroyer = nullptr;

        template <typename Callable, typename... Args>
        inline static auto InvokeCallable(void *storage, Args &&...args) -> decltype(auto)
        {
            auto &callable = *static_cast<Callable *>(storage);
            return InvokeImpl<Callable, Args...>(callable, std::forward<Args>(args)...);
        }

        template <typename Callable, typename First, typename... Rest>
        inline static auto InvokeImpl(Callable &callable, First &&first, Rest &&...rest) -> decltype(auto)
        {
            return callable(std::forward<First>(first), std::forward<Rest>(rest)...);
        }

        template <typename Callable>
        inline static auto InvokeImpl(Callable &callable) -> decltype(auto)
        {
            return callable();
        }

        template <typename Callable>
        static void DestroyCallable(void *storage)
        {
            auto &callable = *static_cast<Callable *>(storage);
            callable.~Callable();
        }
    };
}