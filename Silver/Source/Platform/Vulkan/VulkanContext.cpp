#include "AgPCH.h"
#include "VulkanContext.h"

namespace Silver {

    VulkanContext::VulkanContext()
    {
        AG_CORE_CRITICAL(AG_FUNCTION_NAME);
    }

    VulkanContext::~VulkanContext()
    {
    }

    void VulkanContext::Init(const char** extension, uint32_t extensionCount)
    {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // We need an extension to interface with the window system
        createInfo.enabledExtensionCount = extensionCount;
        createInfo.ppEnabledExtensionNames = extension;

        createInfo.enabledLayerCount = 0;

        // TODO(Milan): Look into those allocators
        if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS)
            AG_BREAKPOINT;

        AG_CORE_INFO("Succesfully created Vulkan Instance");
    }

    void VulkanContext::Shutdown()
    {
        vkDestroyInstance(m_Instance, nullptr);
    }

}