#include "AgPCH.h"
#include "Buffer.h"
#include "VulkanAllocator.h"
#include "RendererContext.h"

namespace Silver {

	VertexBuffer::VertexBuffer(VertexBufferInfo inInfo)
	{
		/**************************
		*      Staging buffer     *
		**************************/
		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.size = inInfo.Size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VmaAllocation stagingBufferAllocation = VulkanAllocator::AllocateBuffer(&stagingBufferInfo, VMA_MEMORY_USAGE_AUTO, 
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT, &stagingBuffer);

		void* data;
		VulkanAllocator::MapMemory(stagingBufferAllocation, &data);
		memcpy(data, inInfo.Data, inInfo.Size);
		VulkanAllocator::UnmapMemory(stagingBufferAllocation);

		/*************************
		*      Vertex Buffer     *
		*************************/
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = inInfo.Size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		m_Allocation = VulkanAllocator::AllocateBuffer(&bufferInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE, 0, &m_Buffer);

		// Copying them
		{
			VkCommandBuffer commandBuffer = RendererContext::Get().CreatePrimaryCommandBuffer(true);

			VkBufferCopy copyRegion{};
			copyRegion.srcOffset = 0; // Optional
			copyRegion.dstOffset = 0; // Optional
			copyRegion.size = inInfo.Size;
			vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_Buffer, 1, &copyRegion);

			RendererContext::Get().FlushCommandBuffer(commandBuffer, true);
		}

		VulkanAllocator::DestroyBuffer(stagingBuffer, stagingBufferAllocation);
	}

	VertexBuffer::~VertexBuffer()
	{
		VulkanAllocator::DestroyBuffer(m_Buffer, m_Allocation);
	}

	void VertexBuffer::Bind(VkCommandBuffer inCommandBuffer)
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(inCommandBuffer, 0, 1, &m_Buffer, offsets);
	}

	IndexBuffer::IndexBuffer(IndexBufferInfo inInfo)
		: m_Size(inInfo.Size), m_IndexType(inInfo.Type)
	{
		/**************************
		*      Staging buffer     *
		**************************/
		VkBufferCreateInfo stagingBufferInfo = {};
		stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		stagingBufferInfo.size = inInfo.Size;
		stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		stagingBufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		VkBuffer stagingBuffer = VK_NULL_HANDLE;
		VmaAllocation stagingBufferAllocation = VulkanAllocator::AllocateBuffer(&stagingBufferInfo, VMA_MEMORY_USAGE_AUTO,
			VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT, &stagingBuffer);

		void* data;
		VulkanAllocator::MapMemory(stagingBufferAllocation, &data);
		memcpy(data, inInfo.Data, inInfo.Size);
		VulkanAllocator::UnmapMemory(stagingBufferAllocation);

		/************************
		*      Index Buffer     *
		************************/
		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = inInfo.Size;
		bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		m_Allocation = VulkanAllocator::AllocateBuffer(&bufferInfo, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE, 0, &m_Buffer);

		// Copying them
		{
			VkCommandBuffer commandBuffer = RendererContext::Get().CreatePrimaryCommandBuffer(true);

			VkBufferCopy copyRegion{};
			copyRegion.srcOffset = 0; // Optional
			copyRegion.dstOffset = 0; // Optional
			copyRegion.size = inInfo.Size;
			vkCmdCopyBuffer(commandBuffer, stagingBuffer, m_Buffer, 1, &copyRegion);

			RendererContext::Get().FlushCommandBuffer(commandBuffer, true);
		}

		VulkanAllocator::DestroyBuffer(stagingBuffer, stagingBufferAllocation);
	}

	IndexBuffer::~IndexBuffer()
	{
		VulkanAllocator::DestroyBuffer(m_Buffer, m_Allocation);
	}

	void IndexBuffer::Bind(VkCommandBuffer inCommandBuffer)
	{
		if (m_IndexType == IndexType::Uint16)
			vkCmdBindIndexBuffer(inCommandBuffer, m_Buffer, 0, VK_INDEX_TYPE_UINT16);
		else if (m_IndexType == IndexType::Uint32)
			vkCmdBindIndexBuffer(inCommandBuffer, m_Buffer, 0, VK_INDEX_TYPE_UINT32);
	}

}