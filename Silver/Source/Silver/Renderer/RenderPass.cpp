#include "AgPCH.h"
#include "RenderPass.h"
#include "RendererContext.h"
#include "Silver/Core/Application.h"

namespace Utils {

	VkSampleCountFlagBits NumberOfSamplesToVulkan(uint32_t inSamples)
	{
		switch (inSamples)
		{
			case 1:	 return VK_SAMPLE_COUNT_1_BIT;
			case 2:	 return VK_SAMPLE_COUNT_2_BIT;
			case 4:	 return VK_SAMPLE_COUNT_4_BIT;
			case 8:	 return VK_SAMPLE_COUNT_8_BIT;
			case 16: return VK_SAMPLE_COUNT_16_BIT;
			case 32: return VK_SAMPLE_COUNT_32_BIT;
			case 64: return VK_SAMPLE_COUNT_64_BIT;
		}

		AG_WARN("Specified incorrect number of samples - {0} samples! Using 1 sample", inSamples);
		return VK_SAMPLE_COUNT_1_BIT;
	}

}

namespace Silver {

	RenderPass::RenderPass(RenderPassInfo info)
		: m_Info(info)
	{
		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = Application::Get().GetSwapchain()->GetFormat();
		colorAttachment.samples = Utils::NumberOfSamplesToVulkan(1);
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		// TODO(Milan): In the future refactor
		// if (framebufferInfo.SwapchainTarget)
		//  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		// else
		// 	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

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
		createInfo.dependencyCount = 1;
		createInfo.pDependencies = &dependency;
		
		VkResult result = vkCreateRenderPass(RendererContext::Get().GetDevice(), &createInfo, nullptr, &m_RenderPass);
		AG_ASSERT(result == VK_SUCCESS, "Failed to create Render Pass!");
	}

	RenderPass::~RenderPass()
	{
		vkDestroyRenderPass(RendererContext::Get().GetDevice(), m_RenderPass, nullptr);
	}

}