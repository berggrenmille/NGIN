#pragma once
#include <NGIN/Defines.hpp>
#include <NGIN/Meta/TypeID.hpp>
#include <NGIN/Meta/FunctionTraits.hpp>
#include <type_traits>
#include <NGIN/Meta/StoragePolicy.hpp>
#include <functional>
namespace NGIN::Util
{
    class DynamicDelegate
    {

    public:
        DynamicDelegate() : storage(), invoker(nullptr), destroyer(nullptr){};

        // Handling Free Functions and Non-capturing Lambdas
        template <typename F>
        DynamicDelegate(F &&f) noexcept
        {
            using CallableType = std::decay_t<F>;
            using Traits = Meta::FunctionTraits<CallableType>;
            using ArgumentTuple = typename Traits::ArgsTupleType;

            returnTypeID = Meta::TypeID<typename Traits::ReturnType>();
            storage = StorageType(CallableType(std::forward<F>(f)));
            SetInvokerFromArgsTuple<CallableType, ArgumentTuple>(Traits::argsIndexSequence);
            destroyer = &DestroyCallable<CallableType>;
        }

        template <typename R, typename... Args>
        DynamicDelegate(R (*func)(Args...)) noexcept
            : returnTypeID(Meta::TypeID<R>())
        {

            using FuncType = R (*)(Args...);
            storage = StorageType(FuncType(func));

            invoker = reinterpret_cast<InvokerFunc<void>>(InvokeCallable<FuncType, Args...>);
            destroyer = &DestroyCallable<FuncType>;
        }

        // Handling Member Functions
        template <class T, typename R, typename... Args>
        DynamicDelegate(R (T::*func)(Args...), T *obj) noexcept
            : returnTypeID(Meta::TypeID<R>())
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

        template <class T, typename R, typename... Args>
        DynamicDelegate(R (T::*func)(Args...) const, const T *obj) noexcept
            : returnTypeID(Meta::TypeID<R>())
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

        ~DynamicDelegate()
        {
            if (destroyer)
            {
                destroyer(storage.get());
            }
        }

        template <typename... Args>
        void operator()(Args... args)
        {

            reinterpret_cast<InvokerFunc<void, Args...>>(invoker)(storage.get(), std::forward<Args>(args)...);
            //   return invoker(storage.get(), args_arr);
        }

        template <typename R, typename... Args>
        R Return(Args... args)
        {
            static Size localReturnTypeID = Meta::TypeID<R>();
            if (localReturnTypeID != returnTypeID)
                throw std::bad_cast();

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
        Size returnTypeID = 0;

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

        template <typename Callable, typename Tuple, std::size_t... I>
        void SetInvokerFromArgsTuple(std::index_sequence<I...>)
        {
            invoker = reinterpret_cast<InvokerFunc<void>>(InvokeCallable<Callable, std::tuple_element_t<I, Tuple>...>);
        }
    };
}