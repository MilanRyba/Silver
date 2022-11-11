#include "AgPCH.h"
#include "Swapchain.h"
#include "Renderer.h"

#include "Platform/Vulkan/VulkanSwapchain.h"

namespace Silver{

	Ref<Swapchain> Swapchain::Create()
	{
		switch (Renderer::CurrentAPI())
		{
			case RendererAPIType::Vulkan: return Ref<VulkanSwapchain>::Create();
		}

		return nullptr;
	}

}
