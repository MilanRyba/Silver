#include "AgPCH.h"
#include "RendererContext.h"
#include "Renderer.h"

#include "Platform/Vulkan/VulkanContext.h"

namespace Silver {

	Ref<RendererContext> RendererContext::Create()
	{
		switch (Renderer::CurrentAPI())
		{
			case RendererAPIType::Vulkan: return Ref<VulkanContext>::Create();
		}

		return nullptr;
	}

}