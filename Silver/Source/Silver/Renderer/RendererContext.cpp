#include "AgPCH.h"
#include "RendererContext.h"

#include "Platform/Vulkan/VulkanContext.h"

namespace Silver {

	Ref<RendererContext> RendererContext::Create(RendererAPIType InType)
	{
		switch (InType)
		{
			case RendererAPIType::Vulkan: return Ref<VulkanContext>::Create();
		}

		return nullptr;
	}

}