#include "AgPCH.h"
#include "Application.h"

namespace Silver {

	Application::Application(const ApplicationSpecification& spec)
		: m_Specification(spec)
	{

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

		}
		OnShutdown();
	}

}