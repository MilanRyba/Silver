#include "ImGuiPlatformContextVulkan.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "Platform/Vulkan/VulkanContext.h"
#include "Platform/Vulkan/VulkanSwapchain.h"

#include "Silver/Core/Application.h"

static ImGui_ImplVulkanH_Window s_MainWindowData;

static void check_vk_result(VkResult err)
{
	if (err == 0)
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
	if (err < 0)
		abort();
}

void ImGuiPlatformContextVulkan::InitPlatform(GLFWwindow* InWindow)
{
	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};

	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;
	vkCreateDescriptorPool(Silver::VulkanContext::Get().GetDevice(), &pool_info, nullptr, &m_DescriptorPool);

	ImGui_ImplGlfw_InitForVulkan(InWindow, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = Silver::VulkanContext::Get().GetInstance();
	init_info.PhysicalDevice = Silver::VulkanContext::Get().GetPhysicalDevice();
	init_info.Device = Silver::VulkanContext::Get().GetDevice();
	init_info.QueueFamily = Silver::VulkanContext::Get().GetGraphicsQueue().QueueFamilyIndex;
	init_info.Queue = Silver::VulkanContext::Get().GetGraphicsQueue().QueueHandle;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = m_DescriptorPool;
	init_info.Subpass = 0;
	init_info.MinImageCount = Silver::Application::Get().GetSwapchain()->GetImageCount();
	init_info.ImageCount = Silver::Application::Get().GetSwapchain()->GetImageCount();
	init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	init_info.Allocator = nullptr;
	init_info.CheckVkResultFn = check_vk_result;
	ImGui_ImplVulkan_Init(&init_info, s_MainWindowData.RenderPass);
}

/*
* According to steps in main.cpp from ImGui_ImplVulkan, implement it here
*/

void ImGuiPlatformContextVulkan::ShutdownPlatform()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
}

void ImGuiPlatformContextVulkan::BeginFramePlatform()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
}

void ImGuiPlatformContextVulkan::EndFramePlatform()
{
	// ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
}
