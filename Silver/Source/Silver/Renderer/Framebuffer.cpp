#include "AgPCH.h"
#include "Framebuffer.h"

namespace Silver {

	Framebuffer::Framebuffer(FramebufferInfo inInfo)
		: m_Info(inInfo)
	{
		// No attachments specified
		if (m_Info.Attachments.Attachments.size() == 0)
		{
			// If there is no image to create the framebuffer from then we assert
			AG_ASSERT(m_Info.ExistingImage != VK_NULL_HANDLE, "Framebuffer creation failed - no attachments or existing image were specified!");

			VkFramebufferCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			// createInfo.renderPass = m_RenderPass;
			createInfo.pAttachments = &m_Info.ExistingImage;
			createInfo.attachmentCount = 1;
			createInfo.width = m_Info.Width;
			createInfo.height = m_Info.Height;
			createInfo.layers = 1;
			createInfo.flags = 0;

			// VkResult result = vkCreateRenderPass(RendererContext::Get().GetDevice(), &createInfo, nullptr, &m_RenderPass);
			// AG_ASSERT(result == VK_SUCCESS, "Failed to create Framebuffer!");
		}
	}

}