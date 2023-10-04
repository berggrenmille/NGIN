//
// Created by Maximiliam Berggren on 04/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Graphics/Window.hpp>
// STL Include(s)
// Other Include(s)
#include <SDL2/SDL.h>

namespace NGIN::Graphics
{
    /// @class SDLWindow
    /// @brief SDL-specific implementation of the Surface interface.
    class SDLWindow : public Window
    {
    public:
        /// @brief Default constructor.
        SDLWindow() = default;

        /// @brief Destructor.
        ~SDLWindow() override;

        /// @brief Initializes the SDL window.
        /// @return true if initialization succeeded, false otherwise.
        Bool Init(WindowConfig& windowConfig) override;

        /// @brief Shutdown the SDL window, releasing any resources.
        void Shutdown() override;

        /// @brief Retrieves the native handle of the SDL window.
        /// @return A pointer to the native SDL_Window handle.
        [[nodiscard]] void* GetNativeHandle() const override;

        /// @brief Gets the dimensions of the SDL window.
        /// @param[out] width The width of the SDL window.
        /// @param[out] height The height of the SDL window.
        void GetDimensions(int& outWidth, int& outHeight) const override;

    private:
        void ShutdownImpl();

        SDL_Window* sdlWindow = nullptr;
    };
}


