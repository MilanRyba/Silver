#include "SilverEditor.h"

SilverEditor::SilverEditor(const Silver::ApplicationInfo& InInfo)
	: Application(InInfo)
{
	AG_TRACE("Title of app: {0}", InInfo.Title);

	Silver::Timer shaderTimer;
	m_Shader = new Silver::Shader("Assets/Shaders/Vertex.glsl", "Assets/Shaders/Fragment.glsl");
	AG_INFO("Shader creation took: {0}ms", shaderTimer.ElapsedMillis());

	Silver::Timer renderPassTimer;
	Silver::RenderPassInfo renderPassInfo;
	renderPassInfo.DebugName = "Default";
	m_RenderPass = new Silver::RenderPass(renderPassInfo);
	AG_INFO("Render Pass creation took: {0}ms", renderPassTimer.ElapsedMillis());

	Silver::Timer pipelineTimer;
	Silver::PipelineInfo pipelineInfo;
	pipelineInfo.Shader = m_Shader;
	pipelineInfo.RenderPass = m_RenderPass;
	pipelineInfo.DebugName = "Default";
	m_Pipeline = new Silver::Pipeline(pipelineInfo);
	AG_INFO("Pipeline creation took: {0}ms", pipelineTimer.ElapsedMillis());
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
	Silver::ApplicationInfo info;
	info.Title = "Silver Editor";

	return new SilverEditor(info);
}