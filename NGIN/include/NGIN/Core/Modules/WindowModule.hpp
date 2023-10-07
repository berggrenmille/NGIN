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
#elif NGIN_USE_GLFW
        //using Dependencies = Meta::TypeWrapper<GLFWModule>
#endif
    protected:

    private:

    };
}


