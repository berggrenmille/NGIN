#pragma once

#include "NGIN/Core.h"
#include "NGIN/Graphics/Renderer.hpp"
#include <vulkan/vulkan.hpp>

namespace NGIN::Graphics
{
    class Window;
}

namespace NGIN::Graphics::Vulkan
{
    /**
     * @class Renderer
     * @brief A Renderer implementation using the Vulkan Graphics API.
     *
     * This class provides functionalities for rendering using Vulkan.
     * It manages Vulkan resources such as instances, devices, and surfaces, and provides
     * core rendering functionalities like viewport setting, frame clearing, and more.
     */
    class Renderer final : public ::NGIN::Graphics::Renderer
    {
    public:
        NGIN_API explicit Renderer(Ref <Window> window);

        NGIN_API ~Renderer() override;

        NGIN_API bool Init() override;

        NGIN_API void BeginFrame() override;

        NGIN_API void EndFrame() override;

        NGIN_API void SetViewport(int width, int height) override;

        NGIN_API void Clear(float r, float g, float b, float a) override;

    private:
        bool SetupInstance();

        bool SetupSurface();

        bool SetupDevice();

        bool SetupSwapchain();


        [[nodiscard]] int RateDeviceSuitability(const vk::PhysicalDevice& device) const;

        Ref <Window> window;
        vk::Instance vkInstance;
        vk::SurfaceKHR vkSurface;
        vk::PhysicalDevice vkPhysicalDevice;
        vk::Device vkDevice;
        vk::SwapchainKHR vkSwapchain;
        vk::Format vkSwapchainImageFormat;
        std::vector<vk::Image> swapchainImages;
        std::vector<vk::ImageView> swapchainImageViews;



        // ... Other Vulkan-specific resources (e.g., command buffers, pipeline states, etc.)
    };

}
