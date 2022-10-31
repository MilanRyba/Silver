#include <Silver.h>
#include <Silver/EntryPoint.h>
#include <iostream>

class SandboxApp : public Silver::Application
{
public:
	SandboxApp(const Silver::ApplicationSpecification& InSpecification)
		: Application(InSpecification)
	{
		std::cout << InSpecification.Title << std::endl;
	}

	~SandboxApp() {}

	virtual void OnInit() override {}
	virtual void OnShutdown() override {}
};

Silver::Application* Silver::CreateApplication(int ArgC, char** ArgV)
{
	Silver::ApplicationSpecification spec;
	spec.Title = "Silver Editor";

	return new SandboxApp(spec);
}