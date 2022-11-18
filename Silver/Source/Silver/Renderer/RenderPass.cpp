#include "AgPCH.h"
#include "RenderPass.h"
#include "RendererContext.h"
#include "Silver/Core/Application.h"

namespace Silver {

	RenderPass::RenderPass(RenderPassInfo info)
	{
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = Application::Get().GetSwapchain()->GetFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		createInfo.attachmentCount = 1;
		createInfo.pAttachments = &colorAttachment;
		createInfo.subpassCount = 1;
		createInfo.pSubpasses = &subpass;
		
		VkResult result = vkCreateRenderPass(RendererContext::Get().GetDevice(), &createInfo, nullptr, &m_RenderPass);
		AG_ASSERT(result == VK_SUCCESS, "Failed to create Render Pass!");
	}

	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(RendererContext::Get().GetDevice(), m_RenderPass, nullptr);
	}

}