#include "AgPCH.h"
#include "Application.h"
#include "Silver/Events/KeyEvents.h"
#include "Timer.h"

#include "glm/glm.hpp"

namespace Silver {

	static Application* s_Application = nullptr;

	Application::Application(const ApplicationSpecification& InSpecification)
		: m_Specification(InSpecification)
	{
		Timer timer;
		s_Application = this;

		WindowSpecification windowSpec;
		windowSpec.Title = m_Specification.Title;
		windowSpec.Width = m_Specification.WindowWidth;
		windowSpec.Height = m_Specification.WindowHeight;
		m_Window = std::make_unique<Window>(windowSpec);
		m_Window->Init();
		m_Window->SetEventCallback(AG_BIND_FN(Application::EventCallback));

		// m_Renderer = new Renderer();

		m_RendererContext = new RendererContext();
		uint32_t extensionCount = 0;
		const char** extensions = m_Window->GetRequiredInstanceExtensions(&extensionCount);
		m_RendererContext->Init(extensions, extensionCount);

		m_Swapchain = new Swapchain();
		m_Swapchain->CreateSurface(m_Window->GetWindow());
		m_Swapchain->RecreateSwapchain();
		// m_Window->Maximize();
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
		OnShutdown();
	}

	void Application::EventCallback(Event& InEvent)
	{
		EventDispatcher dispacther(InEvent);
		dispacther.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& InPressedEvent)
		{
			AG_CORE_WARN("{0}", InEvent);
			return true;
		});

		OnEvent(InEvent);
	}

	Application& Application::Get() { return *s_Application; }

}