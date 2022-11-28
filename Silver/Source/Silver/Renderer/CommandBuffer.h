#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

namespace Silver {

	class CommandPool : public RefTarget
	{
	public:
		CommandPool(uint32_t inQueueFamilyIndex);
		~CommandPool();

		VkCommandPool GetCommandPool() { return m_CommandPool; }
	private:
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
	};

	class CommandBuffer : public RefTarget
	{
	public:
		CommandBuffer(VkCommandBuffer inCommandBuffer) : m_CommandBuffer(inCommandBuffer) {}

		// void Set(VkCommandBuffer inCommandBuffer) { m_CommandBuffer = inCommandBuffer; }
		VkCommandBuffer GetCommandBuffer() const { return m_CommandBuffer; }

		void Begin();
		void End();

		void Reset();
	private:
		VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
	};

}