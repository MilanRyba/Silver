#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

namespace Silver {

	// TODO(Milan): In the future create RendererCapabilities

	struct Queue
	{
		VkQueue QueueHandle;
		uint32_t QueueFamilyIndex = UINT32_MAX;
	};

	class RendererContext : public RefTarget
	{
	public:
		RendererContext();
		~RendererContext();

		void Init(const char** extensions, uint32_t extensionCount);
		void Shutdown();

		static RendererContext& Get();
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