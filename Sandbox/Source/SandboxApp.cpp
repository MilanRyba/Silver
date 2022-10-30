#include <Silver.h>
#include <Silver/EntryPoint.h>
#include <iostream>

class SandboxApp : public Silver::Application
{
public:
	SandboxApp(const Silver::ApplicationSpecification& spec)
		: Application(spec)
	{
		std::cout << spec.Title << std::endl;
	}

	~SandboxApp() {}

	virtual void OnInit() override {}
	virtual void OnShutdown() override {}
};

Silver::Application* Silver::CreateApplication(int ArgC, char** ArgV)
{
	Silver::ApplicationSpecification spec;
	spec.Title = "Sandbox";

	return new SandboxApp(spec);
}