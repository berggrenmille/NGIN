// EditorApp.cpp

#include <NGIN.h>
#include <NGIN/Meta/Reflection/Registry.hpp>
#include <NGIN/Core/Engine.hpp>
#include <NGIN/Core/Modules/WindowModule.hpp>
#include <NGIN/EntryPoint.hpp>


int main(int argc, char* argv[])
{
    std::cout << "TEST" << std::endl;
    NGIN::Core::Engine engine {};
    engine.AddModule<NGIN::Core::WindowModule>();
    engine.Init();
    engine.Run();
    return 0;
}