#include "AgPCH.h"
#include "Shader.h"
#include "Renderer.h"

#include "Platform/Vulkan/VulkanShader.h"

namespace Silver {

	Ref<Shader> Shader::Create(std::string_view InVertex, std::string_view InFragment)
	{
		switch (Renderer::CurrentAPI())
		{
			case RendererAPIType::Vulkan: return Ref<VulkanShader>::Create(InVertex, InFragment);
		}

		return nullptr;
	}

}
