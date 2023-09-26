#pragma once
#include <NGIN/Defines.hpp>
#include <vector>
#include <unordered_map>
#include <NGIN/Meta/TypeName.hpp>
#include "Module.hpp"
#include <NGIN/Time.hpp>
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


    private:
        std::unordered_map<String, Int32> moduleIndexMap;
        std::vector<Module*> moduleVector;
        Time::Timer timer = Time::Timer();
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
}
