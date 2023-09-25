#pragma once
#include <NGIN/Defines.hpp>
#include <vector>
#include <unordered_map>
#include <NGIN/Meta/TypeName.hpp>
#include "Layer.hpp"
namespace NGIN::Core
{
    //  class Layer;

    NGIN_API class Engine
    {
    public:
        Engine() = default;
        ~Engine() = default;

        void Run();

        template <typename T>
            requires std::is_base_of_v<Layer, T>
        void AddLayer();


    private:
        std::unordered_map<String, Int32> layerIndexMap;
        std::vector<Layer*> layersVector;
    };





    //Template Implementations
    template <typename T>
    void AddLayer()
    {
        // Check if layer already exists
        if (!layerIndexMap.contains(String(NGIN::Meta::TypeName<T>::Class())))
            return;
        // Check if layer has dependencies
        if constexpr (requires {T::Dependencies(nullptr);})
            T::Dependencies(this);
        // Check if layer already exists again in case of circular dependencies
        if (!layerIndexMap.contains(String(NGIN::Meta::TypeName<T>::Class())))
            return;

        // Create layer
        layerIndexMap[String(NGIN::Meta::TypeName<T>::Class())] = LayersVector.size();
        layers.emplace_back(new T());
    }
}
