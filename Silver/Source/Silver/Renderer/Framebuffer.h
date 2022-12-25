#pragma once
#include "Silver/Core/Reference.h"
#include <vulkan/vulkan.h>

namespace Silver {

	// TODO(Milan): Move to Image file in the future
	enum class ImageFormat
	{
		RGBA8
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
		FramebufferAttachmentInfo(std::initializer_list<ImageFormat> inAttachments) : Attachments(inAttachments) {}

		std::vector<ImageFormat> Attachments;
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

	// TODO(Milan): To be implemented later
	class Framebuffer : public RefTarget
	{
	public:
		Framebuffer(FramebufferInfo inInfo);

		VkFramebuffer GetFramebuffer() const { return m_Framebuffer; }
		FramebufferInfo GetInfo() const { return m_Info; }
	private:
		VkFramebuffer m_Framebuffer = VK_NULL_HANDLE;

		FramebufferInfo m_Info;
	};

}