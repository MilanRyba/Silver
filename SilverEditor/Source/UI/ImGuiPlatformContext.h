#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <vulkan/vulkan.h>

// class ImGuiContext is already taken by ImGui :)
class ImGuiPlatformContext
{
public:
	void Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();
private:
	VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
	VkRenderPass m_RenderPass = VK_NULL_HANDLE;
};