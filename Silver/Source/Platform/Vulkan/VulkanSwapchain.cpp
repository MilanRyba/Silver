#include "AgPCH.h"
#include "VulkanSwapchain.h"
#include "VulkanContext.h"

#include "Silver/Core/Application.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Silver {

	static VkSurfaceFormatKHR ChooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
	{
		for (const auto& availableFormat : availableFormats)
		{
			if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				return availableFormat;
		}

		return availableFormats[0];
	}

	static VkPresentModeKHR ChooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
	{
		for (const auto& availablePresentMode : availablePresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				return availablePresentMode;
		}

		return VK_PRESENT_MODE_FIFO_KHR;
	}

	static VkExtent2D ChooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			return capabilities.currentExtent;
		else
		{
			int width, height;
			glfwGetFramebufferSize(Application::Get().GetWindow()->GetWindow(), &width, &height);

			VkExtent2D actualExtent = {
				static_cast<uint32_t>(width),
				static_cast<uint32_t>(height)
			};

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}
	}

	VulkanSwapchain::VulkanSwapchain()
	{
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		vkDestroySwapchainKHR(VulkanContext::Get().GetDevice(), m_Swapchain, nullptr);
		vkDestroySurfaceKHR(VulkanContext::Get().GetInstance(), m_Surface, nullptr);
	}

	void VulkanSwapchain::CreateSurface(GLFWwindow* InWindow)
	{
		glfwCreateWindowSurface(VulkanContext::Get().GetInstance(), InWindow, nullptr, &m_Surface);
		AG_CORE_WARN("Created Surface! {0}", AG_FUNCTION_NAME);
	}

	void VulkanSwapchain::RecreateSwapchain()
	{
		// Checks for WSI support
		VkPhysicalDevice physicalDevice = VulkanContext::Get().GetPhysicalDevice();
		VkDevice device = VulkanContext::Get().GetDevice();
		uint32_t graphicsQueueIndex = VulkanContext::Get().GetGraphicsQueue().QueueFamilyIndex;
		VkBool32 supported;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, graphicsQueueIndex, m_Surface, &supported);
		AG_ASSERT(supported == VK_TRUE);

		SwapchainSupportDetails details = QuerySwapchainSupport(physicalDevice);
		VkSurfaceFormatKHR surfaceFormat = ChooseSwapchainSurfaceFormat(details.Formats);
		VkPresentModeKHR presentMode = ChooseSwapchainPresentMode(details.PresentModes);
		VkExtent2D extent = ChooseSwapchainExtent(details.Capabilities);

		// How many images will be in the swapchain
		uint32_t imageCount = details.Capabilities.minImageCount + 1;
		if (details.Capabilities.maxImageCount > 0 && imageCount > details.Capabilities.maxImageCount)
			imageCount = details.Capabilities.maxImageCount;

		VkSwapchainCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;

		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = surfaceFormat.format;
		createInfo.imageColorSpace = surfaceFormat.colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;

		createInfo.preTransform = details.Capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = presentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE; // Only one swapchain :)

		VkResult result = vkCreateSwapchainKHR(VulkanContext::Get().GetDevice(), &createInfo, nullptr, &m_Swapchain);
		AG_ASSERT(result == VK_SUCCESS, "Failed to create Vulkan Swapchain!");
		AG_CORE_WARN("Created Swapchain!");

		// Retrieve the handles for swapchain images
		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, nullptr);
		m_Images.resize(imageCount);
		vkGetSwapchainImagesKHR(device, m_Swapchain, &imageCount, m_Images.data());

		m_Format = surfaceFormat.format;
		m_Extent = extent;

		// Create swapchain image views
		m_ImageViews.resize(m_Images.size());
		for (int i = 0; i < m_ImageViews.size(); i++)
		{
			VkImageViewCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_Images[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_Format;

			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			VkResult result = vkCreateImageView(device, &createInfo, nullptr, &m_ImageViews[i]);
			AG_ASSERT(result == VK_SUCCESS, "Failed to create Swapchain Image View!");
		}
		AG_CORE_WARN("Created all Swapchain Image Views!");
	}

	// TODO(Milan): Make static function
	SwapchainSupportDetails VulkanSwapchain::QuerySwapchainSupport(VkPhysicalDevice device)
	{
		SwapchainSupportDetails details;

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.Capabilities);

		uint32_t formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr);
		if (formatCount != 0)
		{
			details.Formats.resize(formatCount);
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.Formats.data());
		}

		uint32_t presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr);
		if (presentModeCount != 0)
		{
			details.PresentModes.resize(presentModeCount);
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &formatCount, details.PresentModes.data());
		}

		return details;
	}

}