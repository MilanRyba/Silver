#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

namespace Silver {

	struct RenderPassInfo
	{
		// Ref<Framebuffer> TargetFramebuffer;
		std::string DebugName;
	};

	class RenderPass : public RefTarget
	{
	public:
		RenderPass(RenderPassInfo info);
		~RenderPass();

		VkRenderPass GetRenderPass() const { return m_RenderPass; }
	private:
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
	};

}