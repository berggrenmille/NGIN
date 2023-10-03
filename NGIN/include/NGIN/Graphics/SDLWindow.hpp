//
// Created by Maximiliam Berggren on 04/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::Graphics
{
    class SDLWindow 
    {
    public:
        NGIN_API SDLWindow() = default;
        NGIN_API SDLWindow(const SDLWindow& other) = default;
        NGIN_API SDLWindow& operator=(const SDLWindow& other) = default;
        NGIN_API SDLWindow(SDLWindow&& other) = default;
        NGIN_API SDLWindow& operator=(SDLWindow&& other) = default;
    protected:
    private:
    };
}


