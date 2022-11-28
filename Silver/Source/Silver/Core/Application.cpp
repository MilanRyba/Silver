#include "AgPCH.h"
#include "Application.h"
#include "Silver/Events/KeyEvents.h"
#include "Timer.h"

#include "glm/glm.hpp"

namespace Silver {

	static Application* s_Application = nullptr;

	Application::Application(const ApplicationInfo& inInfo)
		: m_Info(inInfo)
	{
		Timer timer;
		s_Application = this;

		WindowInfo windowInfo;
		windowInfo.Title = m_Info.Title;
		windowInfo.Width = m_Info.WindowWidth;
		windowInfo.Height = m_Info.WindowHeight;
		m_Window = std::make_unique<Window>(windowInfo);
		m_Window->Init();
		m_Window->SetEventCallback(AG_BIND_FN(Application::EventCallback));

		// ???
		// Renderer::SetConfig(m_Info.RendererConfig);

		m_RendererContext = new RendererContext();
		uint32_t extensionCount = 0;
		const char** extensions = m_Window->GetRequiredInstanceExtensions(&extensionCount);
		m_RendererContext->Init(extensions, extensionCount);

		m_Swapchain = new Swapchain();
		m_Swapchain->CreateSurface(m_Window->GetWindow());
		m_Swapchain->RecreateSwapchain();
		if (m_Info.StartMaximized)
			m_Window->Maximize();
		AG_CORE_ERROR("Application creation took: {0}ms", timer.ElapsedMillis());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		m_Running = true;
		OnInit();
		while (m_Running)
		{
			m_Window->ProcessEvents();

			OnUpdate(m_TimeStep);

			TimePoint time = std::chrono::high_resolution_clock::now();
			m_FrameTime = std::chrono::duration_cast<std::chrono::duration<float>>(time - m_LastFrameTime).count();
			m_TimeStep = glm::min<float>(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;

			m_Running = !m_Window->ShouldClose();
		}
		m_RendererContext->WaitForGPU();
		OnShutdown();
	}

	void Application::EventCallback(Event& inEvent)
	{
		EventDispatcher dispacther(inEvent);
		dispacther.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& InPressedEvent)
		{
			AG_CORE_WARN("{0}", inEvent);
			return true;
		});

		OnEvent(inEvent);
	}

	Application& Application::Get() { return *s_Application; }

}