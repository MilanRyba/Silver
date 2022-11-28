#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

#include "CommandBuffer.h"

namespace Silver {

	// TODO(Milan): In the future create RendererCapabilities

	struct Queue
	{
		VkQueue QueueHandle = VK_NULL_HANDLE;
		uint32_t QueueFamilyIndex = UINT32_MAX;
	};

	class RendererContext : public RefTarget
	{
	public:
		RendererContext();
		~RendererContext();

		void Init(const char** extensions, uint32_t extensionCount);
		void Shutdown();

		// TODO(Milan): Get this situation under control
		VkCommandBuffer CreatePrimaryCommandBuffer();
		void FlushCommandBuffer(VkCommandBuffer inCommandBuffer);

		void WaitForGPU();
		void BeginFrame();
		void EndFrame();

		static RendererContext& Get();
		VkInstance GetInstance() { return m_Instance; }
		VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
		VkDevice GetDevice() { return m_Device; }

		Queue GetGraphicsQueue() { return m_GraphicsQueue; }
		uint32_t GetImageIndex() { return m_ImageIndex; }
	private:
		void FindQueueFamily(VkPhysicalDevice device);
	private:
		VkInstance m_Instance = VK_NULL_HANDLE;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device = VK_NULL_HANDLE;

		Queue m_GraphicsQueue;
		Queue m_ComputeQueue;

		Ref<CommandPool> m_GraphicsCommandPool;
		Ref<CommandPool> m_ComputeCommandPool;

		uint32_t m_ImageIndex = 0;

		VkSemaphore m_ImageReadySemaphore;
		VkSemaphore m_PresentationReadySemaphore;
		VkFence m_FrameFence;
	};

}