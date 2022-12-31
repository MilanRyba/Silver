#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace Silver {

	// TODO(Milan): Move to Image file in the future
	enum class ImageFormat
	{
		None = 0,

		R8, RG8, RGBA8
	};

	struct FramebufferImageInfo
	{
		FramebufferImageInfo() = default;
		FramebufferImageInfo(ImageFormat inFormat) : Format(inFormat) {}

		ImageFormat Format;
		// TODO(Milan): Filtering, wrap
	};

	struct FramebufferAttachmentInfo
	{
		FramebufferAttachmentInfo() = default;
		FramebufferAttachmentInfo(std::initializer_list<FramebufferImageInfo> inAttachments) : Attachments(inAttachments) {}

		std::vector<FramebufferImageInfo> Attachments;
	};

	struct FramebufferInfo
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		glm::vec4 ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		FramebufferAttachmentInfo Attachments;

		// Multisampling
		uint32_t Samples = 1;
		// bool SwapchainTarget = false; --- REMOVE ---

		// VkImageView ExistingImage = VK_NULL_HANDLE; --- ??? ---

		std::string DebugName;
	};

	struct FramebufferAttachment
	{
		VkImage Image;
		VkImageView ImageView;
		VmaAllocation Allocation;
	};

	class Framebuffer : public RefTarget
	{
	public:
		Framebuffer(FramebufferInfo inInfo);

		VkFramebuffer GetFramebuffer() const { return m_Framebuffer; }
		FramebufferInfo GetInfo() const { return m_Info; }
	private:
		VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;

		FramebufferInfo m_Info;

		std::vector<FramebufferImageInfo> m_ColorAttachmentInfos;

		// The actual Vulkan image handles and image views
		std::vector<FramebufferAttachment> m_ColorAttachments;

		// TODO(Milan): In the future depth attachment as well
		// FramebufferAttachment m_DepthAttachment;
	};

}