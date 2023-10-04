#pragma once

#include "Module.hpp"
#include "Engine.hpp"

namespace NGIN::Core
{
    class TestModule2 : public Module
    {
    public:
        TestModule2() = default;

        virtual ~TestModule2() = default;

        virtual void OnTick(const F64 deltaTime) override {};
    };

    class TestModule : public Module
    {
    public:
        using Dependencies = Meta::TypeWrapper<TestModule2>;

        TestModule() = default;

        virtual ~TestModule() = default;

        virtual void OnTick(const F64 deltaTime) override
        {
            std::cout << "TestModule::OnTick(" << deltaTime << ")" << std::endl;
        };
    };


}