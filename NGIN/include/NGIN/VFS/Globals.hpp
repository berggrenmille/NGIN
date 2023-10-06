//
// Created by Maximiliam Berggren on 06/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::VFS
{
    class Path;

    static Bool Init(Int argc, Char** argv);

    static const Path& GetWorkingDirectory();

}


