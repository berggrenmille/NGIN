#pragma once

#include <NGIN/Defines.hpp>

#include <NGIN/Graphics/IContext.hpp>

namespace NGIN::Graphics
{

    class IContext;

    /// class IRenderer
    /// @brief Interface for a renderer.
    class NGIN_API IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        [[nodiscard]] virtual IContext& GetContext() const = 0;
    };

}
