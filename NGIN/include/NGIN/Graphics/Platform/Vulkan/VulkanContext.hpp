//
// Created by Maximiliam Berggren on 07/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Graphics/Context.hpp>
#include "VulkanRenderer.hpp"
#include <vulkan/vulkan.hpp>

// STL Include(s)
// Other Include(s)

namespace NGIN::Graphics
{
    class VulkanContext : public Context
    {
    public:


        Bool Init() override;

        Bool Shutdown() override;

        Bool SetupSwapchain(Ref<Window> window) override;

        [[nodiscard]] Renderer& GetRenderer() const override;

    protected:
    private:
        VulkanRenderer renderer;
        vk::Instance instance;
        vk::PhysicalDevice physicalDevice;
        vk::Device device;

        vk::SurfaceKHR surface;

        Bool SetupInstance();

        Bool SetupSurface();

        Bool SetupDevice();

        [[nodiscard]] static int RateDeviceSuitability(const vk::PhysicalDevice& device);


    };
}


