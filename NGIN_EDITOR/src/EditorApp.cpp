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
    auto& windowModule = engine.AddModule<NGIN::Core::WindowModule>();
    windowModule.CreateWindow({
                                      .name = "Primary",
                                      .title = "Test",
                                      .width = 800,
                                      .height = 600,
                                      .api = NGIN::Graphics::GraphicsAPI::VULKAN,
                                      .resizable = true
                              });

    engine.Init();
    engine.Run();
    return 0;
}