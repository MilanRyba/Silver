#ifdef ag
#pragma once
#include "UI/ImGuiPlatformContext.h"
#include "vulkan/vulkan.h"

class ImGuiPlatformContextVulkan : public ImGuiPlatformContext
{
private:
	virtual void InitPlatform(GLFWwindow* InWindow) override;
	virtual void ShutdownPlatform() override;

	virtual void BeginFramePlatform() override;
	virtual void EndFramePlatform() override;

	VkDescriptorPool m_DescriptorPool;
};

#endif