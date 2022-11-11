#include "SilverEditor.h"

SilverEditor::SilverEditor(const Silver::ApplicationSpecification& InSpecification)
	: Application(InSpecification)
{
	AG_TRACE("Title of app: {0}", InSpecification.Title);

	Silver::Timer timer;
	m_Shader = Silver::Shader::Create("Assets/Shaders/Vertex.glsl", "Assets/Shaders/Fragment.glsl");
	AG_INFO("Shader creation took: {0}ms", timer.ElapsedMillis());
}

SilverEditor::~SilverEditor()
{
}

void SilverEditor::OnInit()
{
	// m_ImGuiContext = ImGuiPlatformContext::Create(Silver::RendererAPIType::Vulkan);
}

void SilverEditor::OnShutdown()
{
}

void SilverEditor::OnUpdate(float deltaTime)
{
}

void SilverEditor::OnEvent(Silver::Event& InEvent)
{
}

Silver::Application* Silver::CreateApplication(int ArgC, char** ArgV)
{
	Silver::ApplicationSpecification spec;
	spec.Title = "Silver Editor";
	spec.RendererAPI = Silver::RendererAPIType::Vulkan;

	return new SilverEditor(spec);
}