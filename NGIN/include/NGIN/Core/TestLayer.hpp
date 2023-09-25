#include "Layer.hpp"
#include <iostream>
#include <NGIN/Core/Engine.hpp>
namespace NGIN::Core
{


    NGIN_API class TestLayer2 : public Layer
    {
    public:
        TestLayer2() = default;
        ~TestLayer2() = default;

        inline void OnAttach() override { std::cout << "TestLayer2 OnAttach" << std::endl; };



    private:

    };
    NGIN_API class TestLayer : public Layer
    {
    public:
        static void Dependencies(Engine* engine)
        {
            engine->AddLayer<TestLayer2>();
        }
        TestLayer() = default;
        ~TestLayer() = default;

        inline void OnAttach() override { std::cout << "TestLayer OnAttach" << std::endl; };



    private:

    };
}