#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

/*
* One resource that helped me implement ImGui with Vulkan and Glfw
* https://frguthmann.github.io/posts/vulkan_imgui/
* 
* Also one bug I encountered because I am rendering ImGui directly into swapchain was that the UI was more white and also was getting
* validation errors about incompatible render passes and pipelines.
* The issue was that the surface format used SRGB and not regular RGB (see the link below or Swapchain.h)
* https://stackoverflow.com/questions/61231840/imgui-vulkan-validation-layer-error-when-undocking
*/

// Also, if you are wondering, class ImGuiContext is already taken by ImGui
class ImGuiLayer
{
public:
	void Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();
private:
	VkDescriptorPool m_DescriptorPool = VK_NULL_HANDLE;
};