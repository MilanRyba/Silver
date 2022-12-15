#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

// TODO(Milan): In the future maybe use Silver Render Pass
// #include "RenderPass.h"

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
		void Create();
		void RecreateSwapchain();

		/* These two start and end the m_RenderPass
		*/
		void Bind(VkCommandBuffer inCommandBuffer);
		void Unbind(VkCommandBuffer inCommandBuffer);

		void AcquireNextImage(VkSemaphore& inImageReadySemaphore);
		void Present(VkSemaphore inFinishedSemaphore);

		VkSwapchainKHR GetSwapchain() const { return m_Swapchain; }
		VkRenderPass GetRenderPass() const { return m_RenderPass; }

		const std::vector<VkImage>& GetImages() const { return m_Images; }
		const std::vector<VkImageView>& GetImageViews() const { return m_ImageViews; }
		const VkFramebuffer GetCurrentFramebuffer() const { return m_Framebuffers[m_ImageIndex]; }

		uint32_t GetImageCount() const { return (uint32_t)m_Images.size(); }
		VkExtent2D GetExtent() const { return m_Extent; }
		VkFormat GetFormat() const { return m_Format; }
	private:
		void DestroySwapchain();

		SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device);
	private:
		 VkSurfaceKHR m_Surface = VK_NULL_HANDLE;
		 VkSwapchainKHR m_Swapchain = VK_NULL_HANDLE;

		 // Index to the currently acquired image from swapchain
		 uint32_t m_ImageIndex = 0;

		 // Swapchain resources
		 std::vector<VkImage> m_Images;
		 std::vector<VkImageView> m_ImageViews;
		 std::vector<VkFramebuffer> m_Framebuffers;

		 VkFormat m_Format;
		 VkExtent2D m_Extent;
		 VkRenderPass m_RenderPass = VK_NULL_HANDLE;
	};

}