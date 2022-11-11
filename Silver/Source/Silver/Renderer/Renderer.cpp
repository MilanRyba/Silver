#include "AgPCH.h"
#include "Renderer.h"

#include "Platform/Vulkan/VulkanRenderer.h"

namespace Silver {

	Ref<Renderer> Silver::Renderer::Create(RendererAPIType InType)
	{
		s_CurrentAPI = InType;

		switch (InType)
		{
			case RendererAPIType::Vulkan: return Ref<VulkanRenderer>::Create();
		}

		return nullptr;
	}

}
