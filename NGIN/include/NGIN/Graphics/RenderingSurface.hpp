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
    class RenderingSurface 
    {
    public:
        NGIN_API RenderingSurface() = default;
        NGIN_API RenderingSurface(const RenderingSurface& other) = default;
        NGIN_API RenderingSurface& operator=(const RenderingSurface& other) = default;
        NGIN_API RenderingSurface(RenderingSurface&& other) = default;
        NGIN_API RenderingSurface& operator=(RenderingSurface&& other) = default;
    protected:
    private:
    };
}


