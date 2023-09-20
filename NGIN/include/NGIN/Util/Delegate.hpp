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
            invoker = reinterpret_cast<InvokerFunc>(&InvokeCallable<CallableType>);
            destroyer = &DestroyCallable<CallableType>;
        }

        template <typename R, typename... Args>
        Delegate(R (*func)(Args...)) noexcept
        {

            using FuncType = R (*)(Args...);
            storage = StorageType(FuncType(func));

            invoker = reinterpret_cast<InvokerFunc>(&InvokeCallable<FuncType, Args...>);
            destroyer = &DestroyCallable<FuncType>;
        }

        // Handling Member Functions
        template <class T, typename R, typename... Args>
        Delegate(R (T::*func)(Args...), T *obj) noexcept
        {
            auto wrapper = [func, obj](Args... args)
            {
                return (obj->*func)(std::forward<Args>(args)...);
            };

            using WrapperType = decltype(wrapper);

            storage = StorageType(std::move(wrapper));
            invoker = reinterpret_cast<InvokerFunc>(&InvokeCallable<WrapperType, Args...>);
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
        auto operator()(Args &&...args) -> decltype(auto)
        {
            if (!invoker)
            {
                throw std::runtime_error("Delegate not initialized");
            }
            void *args_arr[] = {nullptr, &args...};
            return invoker(storage.get(), args_arr);
        }

    private:
        static constexpr size_t BUFFER_SIZE = sizeof(void *) * 4;

        using StorageType = Meta::StoragePolicy::HybridStorage<BUFFER_SIZE>;
        using InvokerFunc = void (*)(void *, void *[]);
        using DestroyerFunc = void (*)(void *);

        StorageType storage;
        InvokerFunc invoker = nullptr;
        DestroyerFunc destroyer = nullptr;

        template <typename Callable, typename... Args>
        static void InvokeCallable(void *storage, void *args_arr[])
        {
            auto &callable = *static_cast<Callable *>(storage);
            InvokeImpl<Callable, Args...>(callable, args_arr);
        }

        template <typename Callable, typename First, typename... Rest>
        static void InvokeImpl(Callable &callable, void *args_arr[])
        {
            callable(*static_cast<First *>(args_arr[1]), *static_cast<Rest *>(args_arr[2])...);
        }

        template <typename Callable>
        static void InvokeImpl(Callable &callable, void *args_arr[])
        {
            callable();
        }

        template <typename Callable>
        static void DestroyCallable(void *storage)
        {
            auto &callable = *static_cast<Callable *>(storage);
            callable.~Callable();
        }
    };
}