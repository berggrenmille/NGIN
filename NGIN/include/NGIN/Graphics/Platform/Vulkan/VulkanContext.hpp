//
// Created by Maximiliam Berggren on 07/10/2023.
//

#pragma once
// NGIN Include(s)
#include <NGIN/Defines.hpp>
#include <NGIN/Graphics/Context.hpp>
// STL Include(s)
// Other Include(s)

namespace NGIN::Graphics
{
    class VulkanContext : public Context
    {
    public:
        NGIN_API VulkanContext() = default;

        NGIN_API VulkanContext(const VulkanContext& other) = default;

        NGIN_API VulkanContext& operator=(const VulkanContext& other) = default;

        NGIN_API VulkanContext(VulkanContext&& other) = default;

        NGIN_API VulkanContext& operator=(VulkanContext&& other) = default;

    protected:
    private:
    };
}


