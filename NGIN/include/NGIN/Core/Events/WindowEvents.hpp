#pragma once

#include <NGIN/Defines.hpp>

namespace NGIN::Core::Events
{
    ///Event that is fired when the window is closed.
    struct WindowCloseEvent
    {
        WindowCloseEvent() = default;
    };


    ///Event that is fired when the window is resized.
    struct WindowResizeEvent
    {
        WindowResizeEvent(UInt32 width, UInt32 height) : width(width), height(height)
        {}

        UInt32 width = 0;
        UInt32 height = 0;
    };

    ///Event that is fired when the window is moved.
    struct WindowMoveEvent
    {
        WindowMoveEvent(UInt32 x, UInt32 y) : x(x), y(y)
        {}

        UInt32 x = 0;
        UInt32 y = 0;
    };

    ///Event that is fired when the window is minimized.
    struct WindowMinimizeEvent
    {
        WindowMinimizeEvent() = default;
    };

    ///Event that is fired when the window is maximized.
    struct WindowMaximizeEvent
    {
        WindowMaximizeEvent() = default;
    };

    ///Event that is fired when the window is restored.
    struct WindowRestoreEvent
    {
        WindowRestoreEvent() = default;
    };

    ///Event that is fired when the window gains focus.
    struct WindowFocusEvent
    {
        WindowFocusEvent() = default;
    };

    ///Event that is fired when the window loses focus.
    struct WindowLostFocusEvent
    {
        WindowLostFocusEvent() = default;
    };


}