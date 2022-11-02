#pragma once
#include "Silver/Renderer/RendererContext.h"

#include <vulkan/vulkan.h>

namespace Silver {

	class VulkanContext : public RendererContext
	{
	public:
		VulkanContext();
		~VulkanContext();

		virtual void Init(const char** extension, uint32_t extensionCount) override;
		virtual void Shutdown() override;
	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;
	};

}