#pragma once
#include <string>
#include <stdint.h>
#include <memory>

#include "Window.h"
#include "Silver/Events/Event.h"

namespace Silver {

	struct ApplicationSpecification
	{
		std::string Title;
		uint32_t WindowWidth = 1920;
		uint32_t WindowHeight = 1080;
	};

	class Application
	{
	public:
		Application(const ApplicationSpecification& InSpecification);
		virtual ~Application();

		void Run();
	private:
		virtual void OnInit() {}
		virtual void OnUpdate() {}
		virtual void OnShutdown() {}

		void EventCallback(Event& InEvent);
		virtual void OnEvent(Event& InEvent) {}
	private:
		ApplicationSpecification m_Specification;
		bool m_Running = false;

		// TODO(Milan): Maybe use Scope<Window> ??
		std::unique_ptr<Window> m_Window = nullptr;
	};

	Application* CreateApplication(int ArgC, char** ArgV);
}