#pragma once
#include <GLFW/glfw3.h>
#include <memory>

// class ImGuiContext is already taken by ImGui :)
class ImGuiPlatformContext
{
public:
	void Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();

	// static std::unique_ptr<ImGuiPlatformContext> Create(Silver::RendererAPIType InRendererAPI);
private:
	virtual void InitPlatform(GLFWwindow* InWindow) = 0;
	virtual void ShutdownPlatform() = 0;

	virtual void BeginFramePlatform() = 0;
	virtual void EndFramePlatform() = 0;
};