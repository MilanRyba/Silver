#include "AgPCH.h"
#include "RendererContext.h"
#include "Swapchain.h"

#include "Silver/Core/Application.h"

namespace Silver {

    static const std::vector<const char*> s_DeviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    static RendererContext* s_Instance = nullptr;

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

    RendererContext::RendererContext()
    {
        s_Instance = this;
    }

    RendererContext::~RendererContext()
    {
    }

    void RendererContext::Init(const char** extensions, uint32_t extensionCount)
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

                // Searches for the graphics and compute queue
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
            vkGetDeviceQueue(m_Device, m_ComputeQueue.QueueFamilyIndex, 0, &m_ComputeQueue.QueueHandle);
        }

        m_GraphicsCommandPool = new CommandPool(m_GraphicsQueue.QueueFamilyIndex);
        m_ComputeCommandPool = new CommandPool(m_ComputeQueue.QueueFamilyIndex);
    }

    void RendererContext::Shutdown()
    {
        vkDestroyDevice(m_Device, nullptr);
        vkDestroyInstance(m_Instance, nullptr);
    }

    VkCommandBuffer RendererContext::CreatePrimaryCommandBuffer(bool inBegin)
    {
        VkCommandBuffer commandBuffer;

        VkCommandBufferAllocateInfo allocateInfo{};
        allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocateInfo.commandPool = m_GraphicsCommandPool->GetCommandPool();
        allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocateInfo.commandBufferCount = 1;

        VkResult result = vkAllocateCommandBuffers(RendererContext::Get().GetDevice(), &allocateInfo, &commandBuffer);
        AG_ASSERT(result == VK_SUCCESS, "Failed to allocate Command Buffer!");

        if (inBegin)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // TODO(Milan): Should this be really be one time submit
            beginInfo.pInheritanceInfo = nullptr;

            VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
            AG_ASSERT(result == VK_SUCCESS, "Failed to begin recording Command Buffer!");
        }

        return commandBuffer;
    }

    void RendererContext::FlushCommandBuffer(VkCommandBuffer inCommandBuffer, bool inFree)
    {
        AG_ASSERT(inCommandBuffer != VK_NULL_HANDLE, "Tried to submit a command buffer that was not valid!");
        
        VkResult result = vkEndCommandBuffer(inCommandBuffer);
        AG_ASSERT(result == VK_SUCCESS)

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &inCommandBuffer;

        vkQueueSubmit(m_GraphicsQueue.QueueHandle, 1, &submitInfo, VK_NULL_HANDLE);

        vkQueueWaitIdle(m_GraphicsQueue.QueueHandle);

        if (inFree)
        {
            // TODO(Milan): Parameterize the pool
            vkFreeCommandBuffers(m_Device, m_GraphicsCommandPool->GetCommandPool(), 1, &inCommandBuffer);
        }
    }

    void RendererContext::WaitForGPU()
    {
        vkDeviceWaitIdle(m_Device);
    }

    RendererContext& RendererContext::Get() { return *s_Instance; }

    void RendererContext::FindQueueFamily(VkPhysicalDevice device)
    {
        uint32_t count;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                m_GraphicsQueue.QueueFamilyIndex = i;

            if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
                m_ComputeQueue.QueueFamilyIndex = i;

            if (m_GraphicsQueue.QueueFamilyIndex != UINT32_MAX && m_ComputeQueue.QueueFamilyIndex != UINT32_MAX)
                break;

            i++;
        }
    }

}