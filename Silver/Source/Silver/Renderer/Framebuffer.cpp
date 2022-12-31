#include "AgPCH.h"
#include "Framebuffer.h"
#include "VulkanAllocator.h"
#include "RendererContext.h"

namespace Utils {

	static VkSampleCountFlagBits NumberOfSamplesToVulkan(uint32_t inSamples)
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

	static VkFormat ImageFormatToVulkan(Silver::ImageFormat inFormat)
	{
		switch (inFormat)
		{
			case Silver::ImageFormat::R8:		return VK_FORMAT_R8_UNORM;
			case Silver::ImageFormat::RG8:		return VK_FORMAT_R8G8_UNORM;
			case Silver::ImageFormat::RGBA8:	return VK_FORMAT_R8G8B8A8_UNORM;
		}

		AG_ASSERT("Incorrect image format specified!");
		return VK_FORMAT_MAX_ENUM;
	}
}

namespace Silver {

	Framebuffer::Framebuffer(FramebufferInfo inInfo)
		: m_Info(inInfo)
	{
		for (auto attachment : m_Info.Attachments.Attachments)
		{
			// TODO(Milan): Check if the attachment is detph
			m_ColorAttachmentInfos.emplace_back(attachment);
		}

		bool multisample = inInfo.Samples > 1;

		if (m_ColorAttachmentInfos.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentInfos.size());

			// Color attachments
			for (int i = 0; i < m_ColorAttachmentInfos.size(); i++)
			{
				VkImageCreateInfo imageInfo{};
				imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
				imageInfo.imageType = VK_IMAGE_TYPE_2D;
				imageInfo.extent.width = m_Info.Width;
				imageInfo.extent.height = m_Info.Height;
				imageInfo.extent.depth = 1;
				imageInfo.mipLevels = 1;
				imageInfo.arrayLayers = 1;
				imageInfo.format = Utils::ImageFormatToVulkan(m_ColorAttachmentInfos[i].Format);
				imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
				imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				imageInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
				imageInfo.samples = Utils::NumberOfSamplesToVulkan(m_Info.Samples);
				imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

				m_ColorAttachments[i].Allocation = VulkanAllocator::AllocateImage(&imageInfo, VMA_MEMORY_USAGE_AUTO, 0, &m_ColorAttachments[i].Image);

				VkImageViewCreateInfo imageViewInfo = {};
				imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				imageViewInfo.image = m_ColorAttachments[i].Image;
				imageViewInfo.format = Utils::ImageFormatToVulkan(m_ColorAttachmentInfos[i].Format);
				imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				imageViewInfo.subresourceRange.baseMipLevel = 0;
				imageViewInfo.subresourceRange.levelCount = 1;
				imageViewInfo.subresourceRange.baseArrayLayer = 0;
				imageViewInfo.subresourceRange.layerCount = 1;

				VkResult result = vkCreateImageView(RendererContext::Get().GetDevice(), &imageViewInfo, nullptr, &m_ColorAttachments[i].ImageView);
				AG_ASSERT(result == VK_SUCCESS, "Failed to create image view for framebuffer attachment!");
			}
						
			
		}
	}

}