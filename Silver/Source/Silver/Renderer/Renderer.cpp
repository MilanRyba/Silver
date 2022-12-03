#include "AgPCH.h"
#include "Renderer.h"

#include "Silver/Core/Application.h"

namespace Silver {

	static RendererConfig s_Config;

	void Renderer::Init()
	{
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::BeginRenderPass(Ref<CommandBuffer> inCommandBuffer, Ref<RenderPass> inRenderPass, uint32_t inImageIndex)
	{
		VkRenderPassBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = inRenderPass->GetRenderPass();
		beginInfo.framebuffer = Application::Get().GetSwapchain()->GetFramebuffers()[inImageIndex];
		beginInfo.renderArea.offset = { 0, 0 };
		beginInfo.renderArea.extent = Application::Get().GetSwapchain()->GetExtent();

		VkClearValue clearValue = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		beginInfo.pClearValues = &clearValue;
		beginInfo.clearValueCount = 1;

		vkCmdBeginRenderPass(inCommandBuffer->GetCommandBuffer(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::EndRenderPass(Ref<CommandBuffer> inCommandBuffer)
	{
		vkCmdEndRenderPass(inCommandBuffer->GetCommandBuffer());
	}

	void Renderer::SetConfig(RendererConfig inConfig) { s_Config = inConfig; }

	RendererConfig Renderer::GetConfig() { return s_Config; }

}