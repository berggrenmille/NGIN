#pragma once

#include <NGIN/Defines.hpp>

namespace NGIN::Core::Events
{
    /// that is fired when the window is closed.
    struct WindowClose
    {
        WindowClose() = default;
    };


    /// that is fired when the window is resized.
    struct WindowResize
    {
        WindowResize(UInt32 width, UInt32 height) : width(width), height(height) {}

        UInt32 width = 0;
        UInt32 height = 0;
    };

    /// that is fired when the window is moved.
    struct WindowMove
    {
        WindowMove(UInt32 x, UInt32 y) : x(x), y(y) {}

        UInt32 x = 0;
        UInt32 y = 0;
    };

    /// that is fired when the window is minimized.
    struct WindowMinimize
    {
        WindowMinimize() = default;
    };

    /// that is fired when the window is maximized.
    struct WindowMaximize
    {
        WindowMaximize() = default;
    };

    /// that is fired when the window is restored.
    struct WindowRestore
    {
        WindowRestore() = default;
    };

    /// that is fired when the window gains focus.
    struct WindowFocus
    {
        WindowFocus() = default;
    };

    /// that is fired when the window loses focus.
    struct WindowLostFocus
    {
        WindowLostFocus() = default;
    };


}