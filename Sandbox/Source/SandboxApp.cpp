#include <Silver.h>
#include <Silver/EntryPoint.h>

#include "glm/glm.hpp"

class SandboxApp : public Silver::Application
{
public:
	SandboxApp(const Silver::ApplicationSpecification& InSpecification)
		: Application(InSpecification)
	{
		AG_INFO("Title of the app: {0}", InSpecification.Title);
	}

	~SandboxApp() {}

	virtual void OnInit() override {}
	virtual void OnShutdown() override {}

	virtual void OnUpdate(float deltaTime) override {}
	virtual void OnEvent(Silver::Event& InEvent) override {}
};

Silver::Application* Silver::CreateApplication(int ArgC, char** ArgV)
{
	Silver::ApplicationSpecification spec;
	spec.Title = "Silver Editor";
	spec.RendererAPI = Silver::RendererAPIType::Vulkan;

	return new SandboxApp(spec);
}