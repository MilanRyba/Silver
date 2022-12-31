#include "Sandbox.h"
#include <imgui.h>

namespace Silver {

	const std::vector<Silver::Vertex> vertices = {
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};

	const std::vector<uint16_t> indices = {
		0, 1, 2, 2, 3, 0
	};

	Sandbox::Sandbox(const Silver::ApplicationInfo& inInfo)
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
		pipelineInfo.RenderPass = m_Swapchain->GetRenderPass();
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
		indexBufferInfo.Type = IndexType::Uint16;
		m_IndexBuffer = new Silver::IndexBuffer(indexBufferInfo);
	}

	Sandbox::~Sandbox()
	{
	}

	void Sandbox::OnInit()
	{
	}

	void Sandbox::OnShutdown()
	{
	}

	void Sandbox::OnUpdate(float inDeltaTime)
	{
		// Waits for the previous frame to finish
		// Acquires next image from swapchain
		Silver::Renderer::BeginFrame();

		// --- DO RENDERING HERE ---
		// have rendering commands as functions inside CommandBuffer class (drawIndexed)

		/*
			* begin command buffer, begin render pass
			* bind pipeline, vertex and index buffer
			* viewport adn scissor
			* draw
			* end render pass, end command buffer
		*/

		auto cmdBuffer = Silver::Renderer::GetCurrentCommandBuffer()->GetCommandBuffer();
		m_Swapchain->Bind(cmdBuffer); // Begin Render Pass

		// Rendering
		{
			m_Pipeline->Bind(cmdBuffer);

			m_VertexBuffer->Bind(cmdBuffer);
			m_IndexBuffer->Bind(cmdBuffer);

			vkCmdDrawIndexed(cmdBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
		}

		m_Swapchain->Unbind(cmdBuffer); // End Render Pass

		// Submits command buffer. Presents image in the swapchain
		Silver::Renderer::EndFrame();
	}

	void Sandbox::OnEvent(Silver::Event& inEvent)
	{
	}

	Application* Silver::CreateApplication(int ArgC, char** ArgV)
	{
		Silver::ApplicationInfo info;
		info.Title = "Sandbox";
		info.StartMaximized = false;
		info.WindowWidth = 1200;
		info.WindowHeight = 900;

		info.RendererConfig.FramesInFlight = 3;

		return new Sandbox(info);
	}
}
