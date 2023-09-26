#pragma once
#include <NGIN/Defines.hpp>
#include <vector>
#include <unordered_map>
#include <NGIN/Meta/TypeName.hpp>
#include "Module.hpp"
#include <NGIN/Time.hpp>
#include <NGIN/Core/EventBus.hpp>
namespace NGIN::Core
{
    //  class Layer;

    class Engine
    {
    public:
        NGIN_API Engine() = default;
        NGIN_API  ~Engine() = default;

        NGIN_API void Tick();

        template <typename T>
            requires std::is_base_of_v<Module, T>
        void AddModule();

        template <typename T>
            requires std::is_base_of_v<Module, T>
        T* GetModule();


    private:
        std::unordered_map<String, Int32> moduleIndexMap;
        std::vector<Module*> moduleVector;
        Time::Timer timer = Time::Timer();
        EventBus eventBus = EventBus();
    };





    //Template Implementations
    /// TODO: AddModule() should allocate the module from an allocator
    template <typename T>
        requires std::is_base_of_v<Module, T>
    void Engine::AddModule()
    {
        const String TName = String(NGIN::Meta::TypeName<T>::Class());
        // Check if layer already exists
        if (moduleIndexMap.contains(TName))
            return;
        // Check if layer has dependencies
        if constexpr (requires {T::Dependencies(nullptr);})
            T::Dependencies(this);
        // Check if layer already exists again in case of circular dependencies
        if (moduleIndexMap.contains(TName))
            return;

        // Create layer
        moduleIndexMap[TName] = moduleVector.size();
        moduleVector.emplace_back(new T());
    }

    template <typename T>
        requires std::is_base_of_v<Module, T>
    T* Engine::GetModule()
    {
        const String TName = String(NGIN::Meta::TypeName<T>::Class());
        if (!moduleIndexMap.contains(TName))
            return nullptr;
        return static_cast<T*>(moduleVector[moduleIndexMap[TName]]);
    }
}
