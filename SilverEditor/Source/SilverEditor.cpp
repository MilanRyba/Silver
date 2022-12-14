#include "SilverEditor.h"
#include <imgui.h>

SilverEditor::SilverEditor(const Silver::ApplicationInfo& inInfo)
	: Application(inInfo)
{
	AG_TRACE("Title of app: {0}", inInfo.Title);
	Silver::Timer silverEditor;

	m_CommandBuffers.resize(Silver::Renderer::GetConfig().FramesInFlight);
	for (auto& buffer : m_CommandBuffers)
	{
		VkCommandBuffer cmdBuffer = m_RendererContext->CreatePrimaryCommandBuffer();
		buffer = new Silver::CommandBuffer(cmdBuffer);
	}

	Silver::Timer shaderTimer;
	m_Shader = new Silver::Shader("Assets/Shaders/Vertex.glsl", "Assets/Shaders/Fragment.glsl");
	AG_INFO("Shader creation took: {0}ms", shaderTimer.ElapsedMillis());

	Silver::Timer renderPassTimer;
	Silver::RenderPassInfo renderPassInfo;
	renderPassInfo.DebugName = "Default";
	m_RenderPass = new Silver::RenderPass(renderPassInfo);
	AG_INFO("Render Pass creation took: {0}ms", renderPassTimer.ElapsedMillis());
	// m_Swapchain->CreateFramebuffers(m_RenderPass);

	Silver::Timer pipelineTimer;
	Silver::PipelineInfo pipelineInfo;
	pipelineInfo.Shader = m_Shader;
	pipelineInfo.RenderPass = m_RenderPass;
	pipelineInfo.DebugName = "Default";
	m_Pipeline = new Silver::Pipeline(pipelineInfo);
	AG_INFO("Pipeline creation took: {0}ms", pipelineTimer.ElapsedMillis());

	AG_WARN("SilverEditor::SilverEditor took: {0}s", silverEditor.ElapsedSeconds());
}

SilverEditor::~SilverEditor()
{
}

void SilverEditor::OnInit()
{
	m_ImGuiContext.Init();
}

void SilverEditor::OnShutdown()
{
}

// TODO(Milan): Handle swapchain recreation with ImGui
void SilverEditor::OnUpdate(float inDeltaTime)
{
	// Waits for the previous frame to finish
	// Acquires next image from swapchain
	Silver::Renderer::BeginFrame();
	/*
	m_RendererContext->WaitForFrameFence(m_CurrentFrame);

	VkResult beginResult = m_RendererContext->BeginFrame(m_CurrentFrame);
	if (beginResult == VK_ERROR_OUT_OF_DATE_KHR)
	{
		m_Swapchain->RecreateSwapchain(m_RenderPass);
		return;
	}
	else if (beginResult != VK_SUCCESS && beginResult != VK_SUBOPTIMAL_KHR)
		AG_ASSERT("Failed to acquire Swapchain image!");
	m_RendererContext->ResetFrameFence(m_CurrentFrame);
	*/

	// --- DO RENDERING HERE ---
	// have rendering commands as functions inside CommandBuffer class (drawIndexed)

	/*
	m_CommandBuffers[m_CurrentFrame]->Reset();

	m_CommandBuffers[m_CurrentFrame]->Begin();
	Silver::Renderer::BeginRenderPass(m_CommandBuffers[m_CurrentFrame], m_RenderPass, m_RendererContext->GetImageIndex());

	// Rendering
	{
		VkCommandBuffer cmdBuffer = m_CommandBuffers[m_CurrentFrame]->GetCommandBuffer();
		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->GetPipeline());
		// m_Pipeline->Begin() // -> Binds the pipeline

		VkExtent2D extent = m_Swapchain->GetExtent();
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(extent.width);
		viewport.height = static_cast<float>(extent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = extent;
		vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

		vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
	}

	Silver::Renderer::EndRenderPass(m_CommandBuffers[m_CurrentFrame]);
	m_CommandBuffers[m_CurrentFrame]->End();
	// Submits the command buffer to a (for now graphics) queue
	m_RendererContext->FlushCommandBuffer(m_CommandBuffers[m_CurrentFrame]->GetCommandBuffer(), m_CurrentFrame);
	*/

	// Handle ImGui when resizing;
	DrawUI();

	// Presents image in the swapchain
	Silver::Renderer::EndFrame();
	/*
	VkResult endResult = m_RendererContext->EndFrame(m_CurrentFrame);
	if (endResult == VK_ERROR_OUT_OF_DATE_KHR || endResult == VK_SUBOPTIMAL_KHR || m_Resized)
		m_Swapchain->RecreateSwapchain(m_RenderPass);
	else if (endResult != VK_SUCCESS)
		AG_ASSERT("Failed to present Swapchain image!");

	m_CurrentFrame = (m_CurrentFrame + 1) % m_Info.RendererConfig.FramesInFlight;
	*/
}

void SilverEditor::OnEvent(Silver::Event& inEvent)
{
}

void SilverEditor::DrawUI()
{
	m_ImGuiContext.BeginFrame();

	ImGuiViewport* MainViewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(MainViewport->Pos);
	ImGui::SetNextWindowSize(MainViewport->Size);
	ImGui::SetNextWindowViewport(MainViewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGuiWindowFlags DockspaceWindowFlags =
		ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("MainDockspaceWindow", nullptr, DockspaceWindowFlags);
	ImGui::PopStyleVar(3);
	ImGui::DockSpace(ImGui::GetID("MainDockspace"));


	ImGui::ShowDemoWindow();
	
	bool open = true;
	ImGui::Begin("Viewport");
	ImGui::End();

	ImGui::Begin("Outliner", &open);
	ImGui::Text("Point Light");
	ImGui::End();
	
	ImGui::End();
	ImGui::Render();

	auto cmdBuffer = Silver::Renderer::GetCurrentCommandBuffer();
	m_Swapchain->Bind(cmdBuffer->GetCommandBuffer());
	m_ImGuiContext.EndFrame();
	m_Swapchain->Unbind(cmdBuffer->GetCommandBuffer());
}

Silver::Application* Silver::CreateApplication(int ArgC, char** ArgV)
{
	Silver::ApplicationInfo info;
	info.Title = "Silver Editor";
	info.StartMaximized = true;

	info.RendererConfig.FramesInFlight = 2;

	return new SilverEditor(info);
}