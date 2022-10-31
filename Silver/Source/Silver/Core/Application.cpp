#include "AgPCH.h"
#include "Application.h"
#include "Events/KeyEvents.h"

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

		// m_Window->Maximize();

		KeyPressedEvent e(20);
		std::cout << e.ToString() << std::endl;
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

			m_Running = !m_Window->ShouldClose();
		}
		OnShutdown();
	}

	void Application::EventCallback(Event& InEvent)
	{
		std::cout << InEvent.ToString() << std::endl;
	}

}