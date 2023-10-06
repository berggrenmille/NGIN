//
// Created by Maximiliam Berggren on 05/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Core/Module.hpp>
#include <NGIN/Graphics/Window.hpp>

#ifdef NGIN_USE_SDL
#include <NGIN/Graphics/Platform/SDL/SDLWindow.hpp>
#include "NGIN/Core/Platform/SDL/SDLModule.hpp"
#endif

#ifdef NGIN_USE_GLFW
/// TODO: Implement GLFW Window
#endif

// STL Include(s)
// Other Include(s)

namespace NGIN::Core
{
    class WindowModule : public Module
    {
    public:
#ifdef NGIN_USE_SDL
        using Dependencies = Meta::TypeWrapper<SDLModule>;
#elif NGIN_USE_GLFW
        //using Dependencies = Meta::TypeWrapper<GLFWModule>
#endif


    protected:
    protected:
    private:
    };
}


