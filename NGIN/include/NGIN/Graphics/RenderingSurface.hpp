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
    /// @class RenderingSurface
    /// @brief Interface for a rendering target surface.
    class NGIN_API RenderingSurface
    {
    public:
        /// @brief Virtual destructor to ensure proper cleanup for derived classes.
        virtual ~RenderingSurface() = default;

        /// @brief Initializes the surface.
        /// @return true if initialization succeeded, false otherwise.
        virtual bool Init() = 0;

        /// @brief Shutdown the surface, releasing any resources.
        virtual void Shutdown() = 0;

        /// @brief Retrieves the native handle of the surface.
        /// @return A pointer to the native surface handle.
        [[nodiscard]] virtual void* GetNativeHandle() const = 0;

        /// @brief Gets the dimensions of the surface.
        /// @param[out] width The width of the surface.
        /// @param[out] height The height of the surface.
        virtual void GetDimensions(int& width, int& height) const = 0;
    };
}


