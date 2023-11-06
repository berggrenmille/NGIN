#pragma once

#include <NGIN/Defines.hpp>

#include <NGIN/Graphics/IContext.hpp>

namespace NGIN::Graphics
{

    /**
     * @brief Represents a common rendering interface that abstracts the specifics of any graphics API.
     *
     */
    class NGIN_API IRenderer 
    {
    public:
        IRenderer(IContext& context) : context(context) {}
        // Renderer methods that use context...
    protected: 
        IRenderer() = default;
    private:
        IContext& context;
    };

}
