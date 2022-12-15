#include "ImGuiLayer.h"
#include "Silver/Core/Application.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>

#include "Silver/Renderer/RendererContext.h"

static void CheckVulkanError(VkResult inError)
{
	if (inError == 0) // VK_SUCCESS
		return;
	fprintf(stderr, "[vulkan] Error: VkResult = %d\n", inError);
	if (inError < 0)
		abort();
}

void ImGuiLayer::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto/Roboto-Regular.ttf", 16.0f);

	ImGui::StyleColorsDark();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Create Descriptor Pool for ImGui
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
		VkResult result = vkCreateDescriptorPool(Silver::RendererContext::Get().GetDevice(), &pool_info, nullptr, &m_DescriptorPool);
		AG_ASSERT(result == VK_SUCCESS, "Failed to create Descriptor Pool for ImGui!");
	}

	ImGui_ImplGlfw_InitForVulkan(Silver::Application::Get().GetWindow()->GetWindow(), true);

	ImGui_ImplVulkan_InitInfo initInfo{};
	initInfo.Instance = Silver::RendererContext::Get().GetInstance();
	initInfo.PhysicalDevice = Silver::RendererContext::Get().GetPhysicalDevice();
	initInfo.Device = Silver::RendererContext::Get().GetDevice();
	initInfo.QueueFamily = Silver::RendererContext::Get().GetGraphicsQueue().QueueFamilyIndex;
	initInfo.Queue = Silver::RendererContext::Get().GetGraphicsQueue().QueueHandle;
	initInfo.PipelineCache = VK_NULL_HANDLE;
	initInfo.DescriptorPool = m_DescriptorPool;
	initInfo.Allocator = nullptr;
	initInfo.MinImageCount = Silver::Application::Get().GetSwapchain()->GetImageCount(); // ImGui does not actually need the min image count, it just needs to be bigger than 1
	initInfo.ImageCount = Silver::Application::Get().GetSwapchain()->GetImageCount();
	initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	initInfo.CheckVkResultFn = &CheckVulkanError;

	// We use render pass from swapchain
	ImGui_ImplVulkan_Init(&initInfo, Silver::Application::Get().GetSwapchain()->GetRenderPass());

	VkCommandBuffer commandBuffer = Silver::RendererContext::Get().CreatePrimaryCommandBuffer(true);
	ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
	Silver::RendererContext::Get().FlushCommandBuffer(commandBuffer, true);
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	// TODO(Milan): Different theme for ImGui
	// SetDarkThemeColors();
}

void ImGuiLayer::Shutdown()
{
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();

	// vkQueueWaitIdle()

	vkDestroyDescriptorPool(Silver::RendererContext::Get().GetDevice(), m_DescriptorPool, nullptr);
}

void ImGuiLayer::BeginFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::EndFrame()
{
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), Silver::Renderer::GetCurrentCommandBuffer()->GetCommandBuffer());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	// ImGuiIO& io = ImGui::GetIO();
	// if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	// {
	// 	ImGui::UpdatePlatformWindows();
	// 	ImGui::RenderPlatformWindowsDefault();
	// }
}
