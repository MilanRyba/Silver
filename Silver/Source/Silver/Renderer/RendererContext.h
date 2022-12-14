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
		
		/*** TODO(Milan) : Get this situation under control ***/

		/* Allocates a command buffer from command pool(the graphics one)
		* if inBegin is true, it will start recording the command buffer */
		VkCommandBuffer CreatePrimaryCommandBuffer(bool inBegin = false);

		/* Finishes command buffer recording and submits it to a queue
		* if inFree is true, the command buffer will be freed */
		void FlushCommandBuffer(VkCommandBuffer inCommandBuffer, bool inFree = false);

		void WaitForGPU();

		/* THIS IS DONE IN RENDERER*/
		// void WaitForFrameFence(uint32_t inCurrentFrame) { vkWaitForFences(m_Device, 1, &m_FrameFences[inCurrentFrame], VK_TRUE, UINT64_MAX); }
		// void ResetFrameFence(uint32_t inCurrentFrame) { vkResetFences(m_Device, 1, &m_FrameFences[inCurrentFrame]); }
		// VkResult BeginFrame(uint32_t inCurrentFrame);
		// VkResult EndFrame(uint32_t inCurrentFrame);

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

		std::vector<VkSemaphore> m_ImageReadySemaphores;
		std::vector<VkSemaphore> m_PresentationReadySemaphores;
		std::vector<VkFence> m_FrameFences;
	};

}