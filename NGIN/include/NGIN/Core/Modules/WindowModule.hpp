//
// Created by Maximiliam Berggren on 05/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Core/Module.hpp>
#include <NGIN/Graphics/Window.hpp>

#include <NGIN/Graphics/Window.hpp>
#include "NGIN/Core/Modules/SDLModule.hpp"
#include <NGIN/Core/Modules/GraphicsModule.hpp>



// STL Include(s)
// Other Include(s)

namespace NGIN::Core
{
    class WindowModule : public Module
    {
    public:
#ifdef NGIN_USE_SDL
        using Dependencies = Meta::TypeWrapper<SDLModule, GraphicsModule>;
#else
        static_assert(false, "WindowModule has no implementation for this platform");
#endif

        NGIN_API Meta::UUID CreateWindow(const Graphics::WindowConfig& windowConfig);

        NGIN_API void DestroyWindow(const Graphics::Window& window);

        NGIN_API void DestroyWindow(const String& name);

        NGIN_API void DestroyWindow(Meta::UUID id);

        NGIN_API void DestroyAllWindows();

        NGIN_API Graphics::Window& GetWindow(const String& name);

        NGIN_API Graphics::Window& GetWindow(Meta::UUID id);

        NGIN_API std::vector<Graphics::Window>& GetWindows();

    protected:
        NGIN_API void OnInit(Engine* engine) override;

        NGIN_API void OnShutdown() override;


    private:
        Engine        * engine         = nullptr;
        GraphicsModule* graphicsModule = nullptr;
        std::vector<Graphics::Window> windows = {};

    };
}


