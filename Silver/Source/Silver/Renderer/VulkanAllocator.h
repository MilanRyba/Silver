#pragma once
#include "Vma/vk_mem_alloc.h"

/*
* A good reference on different types of buffers, their usages and how to set them up with vma
* https://gpuopen-librariesandsdks.github.io/VulkanMemoryAllocator/html/usage_patterns.html
*/

namespace Silver {

	class VulkanAllocator
	{
	public:
		// Done inside the renderer
		static void Init();
		static void Shutdown();

		static VmaAllocation AllocateBuffer(VkBufferCreateInfo* inBufferInfo, VmaMemoryUsage inUsage, VkMemoryPropertyFlags inMemoryProperties, VkBuffer* outBuffer);

		// Destroys Vulkan buffer and frees allocated memory
		static void DestroyBuffer(VkBuffer inBuffer, VmaAllocation inAllocation);

		static void MapMemory(VmaAllocation inAllocation, void** outData);
		static void UnmapMemory(VmaAllocation inAllocation);
	};

}