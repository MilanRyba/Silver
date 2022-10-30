#pragma once
#include <string>
#include <stdint.h>

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
		Application(const ApplicationSpecification& spec);
		virtual ~Application();

		virtual void OnInit() {}
		virtual void OnShutdown() {}

		void Run();
	private:
		ApplicationSpecification m_Specification;
		bool m_Running = false;
	};

	Application* CreateApplication(int ArgC, char** ArgV);
}