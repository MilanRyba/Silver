#include "AgPCH.h"
#include "Application.h"
#include "Silver/Events/KeyEvents.h"

#include <iostream>

namespace Silver {

	Application::Application(const ApplicationSpecification& InSpecification)
		: m_Specification(InSpecification)
	{
		WindowSpecification windowSpec;
		windowSpec.Title = m_Specification.Title;
		windowSpec.Width = m_Specification.WindowWidth;
		windowSpec.Height = m_Specification.WindowHeight;
		m_Window = std::make_unique<Window>(windowSpec);
		m_Window->Init();
		m_Window->SetEventCallback(AG_BIND_FN(Application::EventCallback));

		m_Window->Maximize();
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
			// TODO(Milan): Should use glm's min()
			m_TimeStep = std::min<float>(m_FrameTime, 0.0333f);
			m_LastFrameTime = time;

			m_Running = !m_Window->ShouldClose();
		}
		OnShutdown();
	}

	void Application::EventCallback(Event& InEvent)
	{
		OnEvent(InEvent);

		// AG_CORE_TRACE("{0}", InEvent.ToString());
	}

}