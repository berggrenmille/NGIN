//
// Created by Maximiliam Berggren on 22/10/2023.
//

//Precompiled include
#include <Precompiled/PCH.h>
// NGIN Include(s)
#include <NGIN/Core/Modules/WindowModule.hpp>
#include <SDL2/SDL.h>
// STL Include(s)
// Other Include(s)

namespace NGIN::Core
{
    void WindowModule::OnInit(Engine* engine)
    {
        this->engine = engine;
        graphicsModule = engine->GetModule<GraphicsModule>();
        NGIN_ASSERT(graphicsModule != nullptr, "GetModule<GraphicsModule> returned nullptr");

    }

    void WindowModule::OnShutdown()
    {
        DestroyAllWindows();
        windows.clear();
    }


    Meta::UUID WindowModule::CreateWindow(const Graphics::WindowConfig& windowConfig)
    {
        windows.emplace_back();
        windows.back().Init(windowConfig);
        return windows.back().GetUUID();
    }

    void WindowModule::DestroyWindow(const Graphics::Window& window)
    {
        for (auto it = windows.begin(); it != windows.end(); ++it)
        {
            if (it->GetUUID() == window.GetUUID())
            {
                it->Shutdown();
                windows.erase(it);
                return;
            }
        }
    }

    void WindowModule::DestroyWindow(const String& name)
    {
        // Find window by name
        auto it = std::find_if
                (
                        windows.begin(),
                        windows.end(),
                        [&name](const Graphics::Window& window) { return window.GetName() == name; }
                );

        if (it != windows.end())
        {
            it->Shutdown();
            windows.erase(it);
        }
    }

    void WindowModule::DestroyWindow(const Meta::UUID id)
    {
        // Find window by UUID
        auto it = std::find_if
                (
                        windows.begin(),
                        windows.end(),
                        [&id](const Graphics::Window& window) { return window.GetUUID() == id; }
                );
        if (it != windows.end())
        {
            it->Shutdown();
            windows.erase(it);
        }
    }

    void WindowModule::DestroyAllWindows()
    {
        for (auto& window: windows)
        {
            window.Shutdown();
        }
        windows.clear();
    }

    Graphics::Window& WindowModule::GetWindow(const String& name)
    {
        // Find window by name
        auto it = std::find_if
                (
                        windows.begin(),
                        windows.end(),
                        [&name](const Graphics::Window& window) { return window.GetName() == name; }
                );
        NGIN_ASSERT(it != windows.end(), "Could not find window with name: {}", name);
        return *it;
    }

    Graphics::Window& WindowModule::GetWindow(Meta::UUID id)
    {
        // Find window by UUID
        auto it = std::find_if
                (
                        windows.begin(),
                        windows.end(),
                        [&id](const Graphics::Window& window) { return window.GetUUID() == id; }
                );
        NGIN_ASSERT(it != windows.end(), "Could not find window with UUID: {}", id.ToString());
        return *it;
    }

    std::vector<Graphics::Window>& WindowModule::GetWindows()
    {
        return windows;
    }
}