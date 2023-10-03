//
// Created by Maximiliam Berggren on 04/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Graphics/RenderingSurface.hpp>
// STL Include(s)
// Other Include(s)
#include <SDL2/SDL.h>

namespace NGIN::Graphics
{
    /// @class SDLWindow
    /// @brief SDL-specific implementation of the RenderingSurface interface.
    class SDLWindow : public RenderingSurface
    {
    public:
        /// @brief Constructs an SDL window with the given title and dimensions.
        /// @param title The title of the window.
        /// @param width The width of the window.
        /// @param height The height of the window.
        SDLWindow(String title, int width, int height);

        /// @brief Destructor.
        virtual ~SDLWindow();

        /// @brief Initializes the SDL window.
        /// @return true if initialization succeeded, false otherwise.
        bool Init() override;

        /// @brief Shutdown the SDL window, releasing any resources.
        void Shutdown() override;

        /// @brief Retrieves the native handle of the SDL window.
        /// @return A pointer to the native SDL_Window handle.
        [[nodiscard]] void* GetNativeHandle() const override;

        /// @brief Gets the dimensions of the SDL window.
        /// @param[out] width The width of the SDL window.
        /// @param[out] height The height of the SDL window.
        void GetDimensions(int& width, int& height) const override;

    private:
        void ShutdownImpl();

        String title;
        Int32 width;
        Int32 height;
        SDL_Window* sdlWindow = nullptr;
    };
}


