#pragma once

#include <NGIN/Defines.hpp>


#include <NGIN/Meta/TypeName.hpp>
#include <NGIN/Meta/TypeWrapper.hpp>

#include "Module.hpp"
#include <NGIN/Time.hpp>
#include <NGIN/Core/EventBus.hpp>
#include <NGIN/Core/Events/Quit.hpp>

#include <unordered_map>
#include <vector>

namespace NGIN::Core
{
    //  class Layer;

    class Engine
    {
    public:
        NGIN_API Engine() = default;

        NGIN_API  ~Engine() = default;

        NGIN_API Bool Init();

        NGIN_API void Tick(F64 delta);

        NGIN_API void Run();


        NGIN_API EventBus& GetEventBus();

        template<typename T, typename ... Args>
        requires std::is_base_of_v<Module, T>
        T& AddModule(Args&& ... args);

        template<typename T>
        requires std::is_base_of_v<Module, T>
        T* GetModule();


        NGIN_API void Quit();


        NGIN_API void Quit(const Events::Quit& event);

    private:

        template<typename... Ts>
        void UnpackModuleDependencies(Meta::TypeWrapper<Ts...>);

        void UnpackModuleDependencies(Meta::TypeWrapper<void>) {}

        std::unordered_map<String, UInt64> moduleIndexMap = {};
        std::vector<Module*>               moduleVector   = {};

        Time::Timer<Time::SteadyClock> timer = Time::Timer<Time::SteadyClock>();

        EventBus eventBus = EventBus();

        Bool shouldQuit    = false;
        Bool isRunning     = false;
        Bool isInitialized = false;
    };


    //Template Implementations
    /// TODO: AddModule() should allocate the module from an allocator
    template<typename T, typename ... Args>
    requires std::is_base_of_v<Module, T>
    T& Engine::AddModule(Args&& ... args)
    {
        const String TName = String(NGIN::Meta::TypeName<T>::Class());

        // Check if layer already exists
        if (moduleIndexMap.contains(TName))
            return static_cast<T&>(*moduleVector[moduleIndexMap[TName]]);

        // Unpack dependencies with compiler magic. Passing empty struct with compile time type information it just makes sense.
        UnpackModuleDependencies(typename T::Dependencies {});

        // Check if layer was created due to circular dependencies
        //TODO: Check if this is the correct way to handle circular dependencies
        if (moduleIndexMap.contains(TName))
            return static_cast<T&>(*moduleVector[moduleIndexMap[TName]]);

        // Create layer
        moduleIndexMap[TName] = moduleVector.size();
        moduleVector.emplace_back(new T(std::forward<Args>(args)...));
        moduleVector.back()->OnInit(this);

        return static_cast<T&>(*moduleVector.back());
    }

    template<typename T>
    requires std::is_base_of_v<Module, T>
    T* Engine::GetModule()
    {
        const String TName = String(NGIN::Meta::TypeName<T>::Class());
        if (!moduleIndexMap.contains(TName))
            return nullptr;
        return static_cast<T*>(moduleVector[moduleIndexMap[TName]]);
    }

    template<typename... Ts>
    void Engine::UnpackModuleDependencies(Meta::TypeWrapper<Ts...>)
    {
        (AddModule<Ts>(), ...);
    }
}
