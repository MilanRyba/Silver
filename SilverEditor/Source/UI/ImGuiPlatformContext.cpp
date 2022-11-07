#include "ImGuiPlatformContext.h"
#include "Silver/Core/Application.h"

#ifdef AG_PLATFORM_WINDOWS
#include "Platform/Vulkan/ImGuiPlatformContextVulkan.h"
#endif

#include <imgui.h>

void ImGuiPlatformContext::Init()
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

	// io.Fonts->AddFontFromFileTTF("assets/fonts/Open_Sans/static/OpenSans/OpenSans-Bold.ttf", 18.0f);
	// io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/Open_Sans/static/OpenSans/OpenSans-Regular.ttf", 18.0f);

	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// SetDarkThemeColors();
	// InitStyle();
	InitPlatform(Silver::Application::Get().GetWindow()->GetWindow());

	// Application& app = Application::Get();
	// GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

	// Setup Platform/Renderer backends
	// ImGui_ImplGlfw_InitForOpenGL(window, true);
	// ImGui_ImplOpenGL3_Init("#version 410");
}

void ImGuiPlatformContext::Shutdown()
{
}

void ImGuiPlatformContext::BeginFrame()
{
	BeginFramePlatform();
	ImGui::NewFrame();
}

void ImGuiPlatformContext::EndFrame()
{
	EndFramePlatform();
}
