//
// Created by Maximiliam Berggren on 01/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Core/Engine.hpp>
#include <NGIN/Core/Module.hpp>
#include <NGIN/Defines.hpp>

#include <NGIN/Graphics/Context.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::Core
{
    class GraphicsModule : public Module
    {
    public:
        NGIN_API Ref<Graphics::Context> GetContext;
        NGIN_API Graphics::GraphicsAPI GetCurrentAPI;
        NGIN_API void SetGraphicsAPI(Graphics::GraphicsAPI api);
        NGIN_API void SetRenderSurface(Graphics::Surface* surface);

    protected:
        NGIN_API void OnInit(Engine* engine) override;

    private:
        Engine* engine = nullptr;
        Ref<Graphics::Context> context = nullptr;
    };
}// namespace NGIN::Core
