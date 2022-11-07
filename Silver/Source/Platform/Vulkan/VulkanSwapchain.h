#pragma once
#include "Silver/Renderer/Swapchain.h"
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace Silver {

	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	class VulkanSwapchain : public Swapchain
	{
	public:
		VulkanSwapchain();
		~VulkanSwapchain();

		virtual void CreateSurface(GLFWwindow* InWindow) override;
		virtual void RecreateSwapchain() override;

		virtual uint32_t GetImageCount() override { return (uint32_t)m_Images.size(); }
	private:
		SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
	private:
		 VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
		 VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;

		 VkFormat m_Format;
		 VkExtent2D m_Extent;

		 std::vector<VkImage> m_Images;
		 std::vector<VkImageView> m_ImageViews;
	};

}