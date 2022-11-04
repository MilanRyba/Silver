#include "AgPCH.h"
#include "VulkanContext.h"
#include "VulkanSwapchain.h"

namespace Silver {

    static const std::vector<const char*> s_DeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    static VulkanContext* s_Instance = nullptr;

    static bool CheckValidationLayerSupport(std::vector<const char*> InLayers)
    {
        uint32_t count;
        vkEnumerateInstanceLayerProperties(&count, nullptr);
        std::vector<VkLayerProperties> layers(count);
        vkEnumerateInstanceLayerProperties(&count, layers.data());

        for (const char* layerName : InLayers)
        {
            bool layerFound = false;

            for (const auto& layerProperties : layers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
                return false;
        }
        return true;
    }

    VulkanContext::VulkanContext()
    {
        s_Instance = this;
    }

    VulkanContext::~VulkanContext()
    {
    }

    void VulkanContext::Init(const char** extensions, uint32_t extensionCount)
    {
        // Create Instance
        {
            VkApplicationInfo appInfo = {};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Silver Engine";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "Silver";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_3;

            VkInstanceCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            createInfo.pApplicationInfo = &appInfo;

            // We need an extension to interface with the window system
            createInfo.enabledExtensionCount = extensionCount;
            createInfo.ppEnabledExtensionNames = extensions;

#ifdef AG_DEBUG
            std::vector<const char*> layers = { "VK_LAYER_KHRONOS_validation" };
            createInfo.enabledLayerCount = 1;
            createInfo.ppEnabledLayerNames = layers.data();
            AG_ASSERT(CheckValidationLayerSupport(layers), "Validation layers requested are not available!")
#endif

            VkResult result = vkCreateInstance(&createInfo, nullptr, &m_Instance);
            AG_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan Instace!")
            AG_CORE_INFO("Succesfully created Vulkan Instance");
        }

        // Choose GPU
        {
            // TODO(Milan): This whole GPU selection should be overhauled
            uint32_t count = 0;
            vkEnumeratePhysicalDevices(m_Instance, &count, nullptr);
            AG_ASSERT(count > 0, "Failed to find GPU with Vulkan support!");

            std::vector<VkPhysicalDevice> devices(count);
            vkEnumeratePhysicalDevices(m_Instance, &count, devices.data());

            for (auto device : devices)
            {
                VkPhysicalDeviceProperties properties;
                vkGetPhysicalDeviceProperties(device, &properties);

                VkPhysicalDeviceFeatures features;
                vkGetPhysicalDeviceFeatures(device, &features);

                // Checks if it is a dedicated graphics card
                if (properties.deviceType != VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                    continue;

                // Checks if the the specified extensions are available (swapchain creation)
                uint32_t extensionCount = 0;
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

                std::vector<VkExtensionProperties> availableExtensions(extensionCount);
                vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

                std::set<std::string> requiredExtensions(s_DeviceExtensions.begin(), s_DeviceExtensions.end());
                for (const auto& extension : availableExtensions)
                    requiredExtensions.erase(extension.extensionName);

                if (!requiredExtensions.empty())
                    continue;

                // For now this just searches for the graphics queue
                FindQueueFamily(device);

                m_PhysicalDevice = device;
                AG_CORE_TRACE("Graphics card selected: {0}", properties.deviceName);
                break;
            }
        }

        // Create Device
        {
            float queuePriority = 1.0f;
            // This structure describes the number of queues we want for a single queue family
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = m_GraphicsQueue.QueueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;

            VkPhysicalDeviceFeatures deviceFeatures{};
            //deviceFeatures.samplerAnisotropy = VK_TRUE;

            VkDeviceCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createInfo.queueCreateInfoCount = 1;
            createInfo.pQueueCreateInfos = &queueCreateInfo;

            createInfo.pEnabledFeatures = &deviceFeatures;

            createInfo.enabledExtensionCount = static_cast<uint32_t>(s_DeviceExtensions.size());
            createInfo.ppEnabledExtensionNames = s_DeviceExtensions.data();

#ifdef AG_DEBUG
            const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
            createInfo.enabledLayerCount = 1;
            createInfo.ppEnabledLayerNames = layers;
#endif

            VkResult result = vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device);
            AG_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan Device!");
            AG_CORE_TRACE("Created Vulkan Device");

            vkGetDeviceQueue(m_Device, m_GraphicsQueue.QueueFamilyIndex, 0, &m_GraphicsQueue.QueueHandle);
        }
    }

    void VulkanContext::Shutdown()
    {
        vkDestroyDevice(m_Device, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }

    VulkanContext& VulkanContext::Get() { return *s_Instance; }

    // TODO(Milan): Refactor
    void VulkanContext::FindQueueFamily(VkPhysicalDevice device)
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                m_GraphicsQueue.QueueFamilyIndex = i;
                break;
            }

            i++;
        }
    }

}