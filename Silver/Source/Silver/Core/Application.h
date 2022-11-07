#pragma once
#include <string>
#include <stdint.h>
#include <memory>
#include <chrono>

#include "Window.h"
#include "Reference.h"
#include "Silver/Events/Event.h"

#include "Silver/Renderer/RendererAPI.h"
#include "Silver/Renderer/RendererContext.h"
#include "Silver/Renderer/Swapchain.h"

namespace Silver {

	struct ApplicationSpecification
	{
		std::string Title;
		uint32_t WindowWidth = 1920;
		uint32_t WindowHeight = 1080;

		RendererAPIType RendererAPI = RendererAPIType::Vulkan;
	};

	class Application
	{
		using TimePoint = std::chrono::high_resolution_clock::time_point;
	public:
		Application(const ApplicationSpecification& InSpecification);
		virtual ~Application();

		void Run();

		static Application& Get();
		std::unique_ptr<Window>& GetWindow() { return m_Window; }
		Ref<Swapchain>& GetSwapchain() { return m_Swapchain; }
	private:
		virtual void OnInit() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnShutdown() {}

		void EventCallback(Event& InEvent);
		virtual void OnEvent(Event& InEvent) {}
	private:
		ApplicationSpecification m_Specification;
		bool m_Running = false;

		TimePoint m_LastFrameTime;
		float m_TimeStep = 0.0f;
		float m_FrameTime = 0.0f;

		// TODO(Milan): Maybe use Scope<Window> ??
		std::unique_ptr<Window> m_Window = nullptr;

		Ref<RendererContext> m_RendererContext = nullptr;
		Ref<Swapchain> m_Swapchain = nullptr;
	};

	Application* CreateApplication(int ArgC, char** ArgV);
}