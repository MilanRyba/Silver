#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

#include "Framebuffer.h"

namespace Silver {

	struct RenderPassInfo
	{
		Ref<Framebuffer> TargetFramebuffer;
		std::string DebugName;
	};

	class RenderPass : public RefTarget
	{
	public:
		RenderPass(RenderPassInfo info);
		~RenderPass();

		VkRenderPass GetRenderPass() const { return m_RenderPass; }
		RenderPassInfo GetInfo() const { return m_Info; }
	private:
		VkRenderPass m_RenderPass = VK_NULL_HANDLE;
		RenderPassInfo m_Info;
	};

}