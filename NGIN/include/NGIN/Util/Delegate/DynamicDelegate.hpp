#pragma once
#include <NGIN/Defines.hpp>
#include <NGIN/Meta/TypeID.hpp>
#include <NGIN/Meta/FunctionTraits.hpp>
#include <type_traits>
#include <NGIN/Meta/StoragePolicy.hpp>
#include <functional>
namespace NGIN::Util
{
    /// \class DynamicDelegate
    /// \brief A dynamic delegate class for type-erased, fast callable invocation.
    class DynamicDelegate
    {
    public:
        /// \brief Default constructor.
        DynamicDelegate() noexcept;

        /// \brief Move constructor.
        DynamicDelegate(DynamicDelegate&& other) noexcept;

        /// \brief Templated constructor.
        template <typename F>
        DynamicDelegate(F&& f) noexcept
            requires (!std::is_same_v<std::decay_t<F>, DynamicDelegate>);

        /// \brief Constructs from function pointers.
        template <typename R, typename... Args>
        DynamicDelegate(R(*func)(Args...)) noexcept;

        /// \brief Constructs from member function pointers.
        template <class T, typename R, typename... Args>
        DynamicDelegate(R(T::* func)(Args...), T* obj) noexcept;

        /// \brief Constructs from const member function pointers.
        template <class T, typename R, typename... Args>
        DynamicDelegate(R(T::* func)(Args...) const, const T* obj) noexcept;

        /// \brief Destructor
        ~DynamicDelegate() = default;

        /// \brief Invokes the stored callable.
        template <typename... Args>
        void operator()(Args&&... args);

        /// \brief Invokes the stored callable with a specified return type.
        template <typename R, typename... Args>
        R Return(Args&&... args);

    private:
        static constexpr size_t BUFFER_SIZE = sizeof(void*) * 4;

        using StorageType = Meta::StoragePolicy::HybridStorage<BUFFER_SIZE>;

        template <typename R, typename... Args>
        using InvokerFunc = R(*)(void*, Args &&...);

        /// \brief Storage for any callable object fitting within BUFFER_SIZE.
        StorageType storage;

        /// \brief Function pointer to invoker.
        InvokerFunc<void> invoker = nullptr;

        /// \brief ID for the return type.
        UInt64 returnTypeID = 0;

        /// \brief Invokes a callable.
        template <typename Callable, typename... Args>
        static auto InvokeCallable(void* storage, Args &&...args) -> decltype(auto);

        /// \brief Sets the invoker based on the argument tuple.
        template <typename Callable, typename Tuple, std::size_t... I>
        void SetInvokerFromArgsTuple(std::index_sequence<I...>);
    };
    inline DynamicDelegate::DynamicDelegate() noexcept : storage(), invoker(nullptr), returnTypeID(0) {}

    inline DynamicDelegate::DynamicDelegate(DynamicDelegate&& other) noexcept
    {
        storage = std::move(other.storage);
        invoker = other.invoker;
        returnTypeID = other.returnTypeID;
        other.invoker = nullptr;
    }

    template <typename F>
    inline DynamicDelegate::DynamicDelegate(F&& f) noexcept
        requires (!std::is_same_v<std::decay_t<F>, DynamicDelegate>)
    {
        using CallableType = std::decay_t<F>;
        using Traits = Meta::FunctionTraits<CallableType>;
        using ArgumentTuple = typename Traits::ArgsTupleType;

        returnTypeID = Meta::TypeID<typename Traits::ReturnType>();
        storage = StorageType(CallableType(std::forward<F>(f)));
        SetInvokerFromArgsTuple<CallableType, ArgumentTuple>(Traits::argsIndexSequence);
    }

    template <typename R, typename... Args>
    inline DynamicDelegate::DynamicDelegate(R(*func)(Args...)) noexcept
        : returnTypeID(Meta::TypeID<R>())
    {
        using FuncType = R(*)(Args...);
        storage = StorageType(FuncType(func));
        invoker = reinterpret_cast<InvokerFunc<void>>(InvokeCallable<FuncType, Args...>);
    }

    template <class T, typename R, typename... Args>
    inline DynamicDelegate::DynamicDelegate(R(T::* func)(Args...), T* obj) noexcept
        : returnTypeID(Meta::TypeID<R>())
    {
        auto wrapper = [func, obj](Args &&...args) {
            return (obj->*func)(std::forward<Args>(args)...);
            };

        using WrapperType = decltype(wrapper);
        storage = StorageType(std::move(wrapper));
        invoker = reinterpret_cast<InvokerFunc<void>>(InvokeCallable<WrapperType, Args...>);
    }

    template <class T, typename R, typename... Args>
    inline DynamicDelegate::DynamicDelegate(R(T::* func)(Args...) const, const T* obj) noexcept
        : returnTypeID(Meta::TypeID<R>())
    {
        auto wrapper = [func, obj](Args &&...args) {
            return (obj->*func)(std::forward<Args>(args)...);
            };

        using WrapperType = decltype(wrapper);
        storage = StorageType(std::move(wrapper));
        invoker = reinterpret_cast<InvokerFunc<void>>(InvokeCallable<WrapperType, Args...>);
    }

    template <typename... Args>
    inline void DynamicDelegate::operator()(Args&&... args)
    {
        reinterpret_cast<InvokerFunc<void, Args...>>(invoker)(storage.get(), std::forward<Args>(args)...);
    }

    template <typename R, typename... Args>
    inline R DynamicDelegate::Return(Args&&... args)
    {
        static UInt64 localReturnTypeID = Meta::TypeID<R>();
        if (localReturnTypeID != returnTypeID)
            throw std::bad_cast();
        return reinterpret_cast<InvokerFunc<R, Args...>>(invoker)(storage.get(), std::forward<Args>(args)...);
    }

    template <typename Callable, typename... Args>
    inline auto DynamicDelegate::InvokeCallable(void* storage, Args &&...args) -> decltype(auto)
    {
        return (*static_cast<Callable*>(storage))(std::forward<Args>(args)...);
    }

    template <typename Callable, typename Tuple, std::size_t... I>
    inline void DynamicDelegate::SetInvokerFromArgsTuple(std::index_sequence<I...>)
    {
        invoker = reinterpret_cast<InvokerFunc<void>>(InvokeCallable<Callable, std::tuple_element_t<I, Tuple>...>);
    }
}