//
// Created by Maximiliam Berggren on 07/10/2023.
//

//Precompiled include
#include <Precompiled/PCH.h>
// NGIN Include(s)
#include <NGIN/Graphics/Platform/Vulkan/VulkanContext.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::Graphics
{
    Bool VulkanContext::SetupInstance()
    {
        return 0;
    }

    Bool VulkanContext::SetupSurface()
    {
        return 0;
    }

    Bool VulkanContext::SetupDevice()
    {
        return 0;
    }

    int VulkanContext::RateDeviceSuitability(const vk::PhysicalDevice& device)
    {
        return 0;
    }
}