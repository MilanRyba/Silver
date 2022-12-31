#include "AgPCH.h"
#include "VulkanAllocator.h"
#include "RendererContext.h"

namespace Silver
{
	struct VulkanAllocatorData
	{
		VmaAllocator Allocator;
	};

	static VulkanAllocatorData* s_AllocatorData = nullptr;

	void VulkanAllocator::Init()
	{
		s_AllocatorData = new VulkanAllocatorData();

		VmaAllocatorCreateInfo createInfo = {};
		createInfo.device = RendererContext::Get().GetDevice();
		createInfo.instance = RendererContext::Get().GetInstance();
		createInfo.physicalDevice = RendererContext::Get().GetPhysicalDevice();
		createInfo.vulkanApiVersion = VK_API_VERSION_1_3;

		VkResult result = vmaCreateAllocator(&createInfo, &s_AllocatorData->Allocator);
		AG_ASSERT(result == VK_SUCCESS, "Failed to create VmaAllocator");
	}

	void VulkanAllocator::Shutdown()
	{
		vmaDestroyAllocator(s_AllocatorData->Allocator);
		delete s_AllocatorData;
		s_AllocatorData = nullptr;
	}

	VmaAllocation VulkanAllocator::AllocateBuffer(VkBufferCreateInfo* inBufferInfo, VmaMemoryUsage inUsage, VkMemoryPropertyFlags inMemoryProperties, VkBuffer* outBuffer)
	{
		VmaAllocationCreateInfo allocationCreateInfo = {};
		allocationCreateInfo.usage = inUsage;
		allocationCreateInfo.flags = inMemoryProperties;

		VmaAllocation outAllocation;
		VmaAllocationInfo allocationInfo = {}; // Optional. Information about allocated memory. It can be later fetched using function vmaGetAllocationInfo()

		VkResult result = vmaCreateBuffer(s_AllocatorData->Allocator, inBufferInfo, &allocationCreateInfo, outBuffer, &outAllocation, &allocationInfo);
		AG_ASSERT(result == VK_SUCCESS);

		AG_CORE_INFO("VulkanAllocator: Allocated buffer of size: {0}", allocationInfo.size);
		return outAllocation;
	}

	VmaAllocation VulkanAllocator::AllocateImage(VkImageCreateInfo* inImageInfo, VmaMemoryUsage inUsage, VkMemoryPropertyFlags inMemoryProperties, VkImage* outImage)
	{
		VmaAllocationCreateInfo allocationCreateInfo = {};
		allocationCreateInfo.usage = inUsage;
		allocationCreateInfo.flags = inMemoryProperties;

		VmaAllocation outAllocation;
		VmaAllocationInfo allocationInfo = {}; // Optional. Information about allocated memory. It can be later fetched using function vmaGetAllocationInfo()

		VkResult result = vmaCreateImage(s_AllocatorData->Allocator, inImageInfo, &allocationCreateInfo, outImage, &outAllocation, &allocationInfo);
		AG_ASSERT(result == VK_SUCCESS);

		AG_CORE_INFO("VulkanAllocator: Allocated image of size: {0}", allocationInfo.size);
		return outAllocation;
	}

	void VulkanAllocator::DestroyBuffer(VkBuffer inBuffer, VmaAllocation inAllocation)
	{
		vmaDestroyBuffer(s_AllocatorData->Allocator, inBuffer, inAllocation);
	}

	void VulkanAllocator::DestroyImage(VkImage inImage, VmaAllocation inAllocation)
	{
		vmaDestroyImage(s_AllocatorData->Allocator, inImage, inAllocation);
	}

	void VulkanAllocator::MapMemory(VmaAllocation inAllocation, void** outData)
	{
		vmaMapMemory(s_AllocatorData->Allocator, inAllocation, outData);
	}

	void VulkanAllocator::UnmapMemory(VmaAllocation inAllocation)
	{
		vmaUnmapMemory(s_AllocatorData->Allocator, inAllocation);
	}
}

