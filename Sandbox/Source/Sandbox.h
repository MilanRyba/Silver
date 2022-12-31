#pragma once
#include <Silver.h>
#include <Silver/EntryPoint.h>

#include "glm/glm.hpp"
#include "UI/ImGuiLayer.h"

namespace Silver {

	class Sandbox : public Application
	{
	public:
		Sandbox(const ApplicationInfo& inInfo);
		~Sandbox();

		virtual void OnInit() override;
		virtual void OnShutdown() override;

		virtual void OnUpdate(float inDeltaTime) override;
		virtual void OnEvent(Event& inEvent) override;

	private:
		uint32_t m_CurrentFrame = 0;

		Ref<Framebuffer> m_Framebuffer;
		Ref<Shader> m_Shader;
		Ref<RenderPass> m_RenderPass;
		Ref<Pipeline> m_Pipeline;

		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}
