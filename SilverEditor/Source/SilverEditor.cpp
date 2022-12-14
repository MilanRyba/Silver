#include "SilverEditor.h"
#include <imgui.h>

const std::vector<Silver::Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};

SilverEditor::SilverEditor(const Silver::ApplicationInfo& inInfo)
	: Application(inInfo)
{
	AG_TRACE("Title of app: {0}", inInfo.Title);
	Silver::Timer silverEditor;

	m_Shader = new Silver::Shader("Assets/Shaders/Vertex.glsl", "Assets/Shaders/Fragment.glsl");

	Silver::FramebufferInfo framebufferInfo;
	framebufferInfo.Attachments = { Silver::ImageFormat::RGBA8 };
	framebufferInfo.Width = 1024;
	framebufferInfo.Height = 1024;
	framebufferInfo.DebugName = "Default";
	m_Framebuffer = new Silver::Framebuffer(framebufferInfo);

	Silver::RenderPassInfo renderPassInfo;
	renderPassInfo.TargetFramebuffer = m_Framebuffer;
	renderPassInfo.DebugName = "Default";
	m_RenderPass = new Silver::RenderPass(renderPassInfo);

	Silver::PipelineInfo pipelineInfo;
	pipelineInfo.DebugName = "Default";
	pipelineInfo.Shader = m_Shader;
	pipelineInfo.Layout = {
		{ Silver::ShaderDataType::Float2, "a_Position"  },
		{ Silver::ShaderDataType::Float3, "a_Color" }
	};
	AG_CRITICAL("{0}", pipelineInfo.Layout.GetStride());
	AG_CRITICAL("{0}", pipelineInfo.Layout.GetAttributes()[0].Name);
	pipelineInfo.RenderPass = m_RenderPass;
	m_Pipeline = new Silver::Pipeline(pipelineInfo);

	AG_WARN("SilverEditor::SilverEditor took: {0}s", silverEditor.ElapsedSeconds());

	Silver::VertexBufferInfo vertexBufferInfo;
	vertexBufferInfo.Size = sizeof(Silver::Vertex) * vertices.size();
	vertexBufferInfo.Data = (void*)vertices.data();
	m_VertexBuffer = new Silver::VertexBuffer(vertexBufferInfo);
	// m_VertexBuffer->SetData(vertices);

	Silver::IndexBufferInfo indexBufferInfo;
	indexBufferInfo.Size = sizeof(indices[0]) * indices.size();
	indexBufferInfo.Data = (void*)indices.data();
	m_IndexBuffer = new Silver::IndexBuffer(indexBufferInfo);
}

SilverEditor::~SilverEditor()
{
}

void SilverEditor::OnInit()
{
	m_ImGuiLayer.Init();

	m_ActiveScene = new Silver::Scene();
	m_Actor1 = m_ActiveScene->CreateActor("Aloy");
	m_Actor2 = m_ActiveScene->CreateActor();

	m_OutlinerPanel.SetContext(m_ActiveScene);
	m_OutlinerPanel.SetSelectedActorChangedCallback(AG_BIND_FN(m_AttributesPanel.ActorChangedCallback));
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

	m_ActiveScene->OnUpdate(inDeltaTime);

	// --- DO RENDERING HERE ---
	// have rendering commands as functions inside CommandBuffer class (drawIndexed)

	// This already happens inside the renderer
	// m_CommandBuffers[m_CurrentFrame]->Reset();
	// m_CommandBuffers[m_CurrentFrame]->Begin();
	
	/*
		* begin command buffer, begin render pass
		* bind pipeline, vertex and index buffer
		* viewport adn scissor
		* draw
		* end render pass, end command buffer
	*/

	/*
	Silver::Renderer::BeginRenderPass(commandBuffer, m_RenderPass);

	// Rendering
	{
		VkCommandBuffer cmdBuffer = m_CommandBuffers[m_CurrentFrame]->GetCommandBuffer();
		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline->GetPipeline());
		// m_Pipeline->Bind()

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

		m_VertexBuffer->Bind(commandBuffer);
		m_IndexBuffer->Bind(commandBuffer);

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
	}

	Silver::Renderer::EndRenderPass(m_CommandBuffer);
	m_CommandBuffers[m_CurrentFrame]->End(); 
	*/
	// Submits the command buffer to a (for now graphics) queue
	// m_RendererContext->FlushCommandBuffer(m_CommandBuffers[m_CurrentFrame]->GetCommandBuffer(), m_CurrentFrame);
	

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
	m_ImGuiLayer.BeginFrame();

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

	ImGui::Begin("MainDockspace", nullptr, DockspaceWindowFlags);
	ImGui::PopStyleVar(3);

	ImGui::DockSpace(ImGui::GetID("MainDockspace"));
	
	bool open = true;
	ImGui::Begin("Viewport");

	// Descritor set with an image
	// ImGui::Image();

	ImGui::End();

	m_OutlinerPanel.OnUIRender();
	m_AttributesPanel.OnUIRender();

	ImGui::End(); // DockSpace
	ImGui::Render();

	auto cmdBuffer = Silver::Renderer::GetCurrentCommandBuffer();
	m_Swapchain->Bind(cmdBuffer->GetCommandBuffer());
	m_ImGuiLayer.EndFrame();
	m_Swapchain->Unbind(cmdBuffer->GetCommandBuffer());
}

Silver::Application* Silver::CreateApplication(int ArgC, char** ArgV)
{
	Silver::ApplicationInfo info;
	info.Title = "Silver Editor";
	info.StartMaximized = true;
	info.WindowWidth = 1280;
	info.WindowHeight = 720;

	info.RendererConfig.FramesInFlight = 2;

	return new SilverEditor(info);
}