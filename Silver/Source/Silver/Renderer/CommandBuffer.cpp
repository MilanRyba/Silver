#include "AgPCH.h"
#include "CommandBuffer.h"

#include "RendererContext.h"

namespace Silver {

	// TODO(Milan): Maybe parameterize the queue
	CommandPool::CommandPool(uint32_t inQueueFamilyIndex)
	{
		auto& context = RendererContext::Get();

		VkCommandPoolCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		createInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		createInfo.queueFamilyIndex = inQueueFamilyIndex;

		VkResult result = vkCreateCommandPool(context.GetDevice(), &createInfo, nullptr, &m_CommandPool);
		AG_ASSERT(result == VK_SUCCESS, "Failed to create Command Pool!");


	}

	CommandPool::~CommandPool()
	{
		vkDestroyCommandPool(RendererContext::Get().GetDevice(), m_CommandPool, nullptr);
	}

	// void CommandPool::AllocateCommandBuffer()
	// {
	// 	VkCommandBuffer commandBuffer;
	// 	// m_CommandBuffers.emplace_back(Command)
	// }

	// CommandBuffer::CommandBuffer()
	// {
	// 	VkCommandBufferAllocateInfo allocateInfo{};
	// 	allocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	// 	allocateInfo.commandPool = /* Need Command Pool */;
	// 	allocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	// 	allocateInfo.commandBufferCount = 1;
	// 
	// 	VkResult result = vkAllocateCommandBuffers(RendererContext::Get().GetDevice(), &allocateInfo, &m_CommandBuffer);
	// 	AG_ASSERT(result == VK_SUCCESS, "Failed to allocate Command Buffers!");
	// }

	void CommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0;
		beginInfo.pInheritanceInfo = nullptr;

		VkResult result = vkBeginCommandBuffer(m_CommandBuffer, &beginInfo);
		AG_ASSERT(result == VK_SUCCESS, "Failed to begin recording Command Buffer!");
	}

	void CommandBuffer::End()
	{
		VkResult result = vkEndCommandBuffer(m_CommandBuffer);
		AG_ASSERT(result == VK_SUCCESS, "Failed to record Command Buffer!");
	}

	void CommandBuffer::Reset()
	{
		vkResetCommandBuffer(m_CommandBuffer, 0);
	}

	void CommandBuffer::DrawIndexed(Ref<IndexBuffer> inIndexBuffer)
	{
		vkCmdDrawIndexed(m_CommandBuffer, static_cast<uint32_t>(inIndexBuffer->GetSize() / inIndexBuffer->GetTypeSize()), 1, 0, 0, 0);
	}

}