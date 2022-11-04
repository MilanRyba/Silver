#include "AgPCH.h"
#include "Swapchain.h"

#include "Platform/Vulkan/VulkanSwapchain.h"

namespace Silver{

	Ref<Swapchain> Swapchain::Create(RendererAPIType InType)
	{
		switch (InType)
		{
			case RendererAPIType::Vulkan: return Ref<VulkanSwapchain>::Create();
		}

		return nullptr;
	}

}
