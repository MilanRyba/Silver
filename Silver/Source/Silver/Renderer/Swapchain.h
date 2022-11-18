#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

struct GLFWwindow;

namespace Silver {

	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	class Swapchain : public RefTarget
	{
	public:
		Swapchain();
		~Swapchain();

		void CreateSurface(GLFWwindow* InWindow);
		void RecreateSwapchain();

		uint32_t GetImageCount() const { return (uint32_t)m_Images.size(); }
		VkExtent2D GetExtent() const { return m_Extent; }
		VkFormat GetFormat() const { return m_Format; }
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