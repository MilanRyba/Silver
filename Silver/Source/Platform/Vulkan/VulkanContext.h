#pragma once
#include "Silver/Renderer/RendererContext.h"

#include <vulkan/vulkan.h>

namespace Silver {

	// TODO(Milan): In the future create RendererCapabilities

	struct Queue
	{
		VkQueue QueueHandle;
		uint32_t QueueFamilyIndex = UINT32_MAX;
	};

	class VulkanContext : public RendererContext
	{
	public:
		VulkanContext();
		~VulkanContext();

		virtual void Init(const char** extension, uint32_t extensionCount) override;
		virtual void Shutdown() override;

		static VulkanContext& Get();
		VkInstance GetInstance() { return m_Instance; }
		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		VkDevice GetDevice() { return m_Device; }

		Queue GetGraphicsQueue() { return m_GraphicsQueue; }
	private:
		void FindQueueFamily(VkPhysicalDevice device);
	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;

		Queue m_GraphicsQueue;
	};

}