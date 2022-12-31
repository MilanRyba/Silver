#pragma once
#include <Silver.h>
#include <Silver/EntryPoint.h>

#include "glm/glm.hpp"
#include "UI/ImGuiLayer.h"
#include "Panels/OutlinerPanel.h"
#include "Panels/AttributesPanel.h"

class SilverEditor : public Silver::Application
{
public:
	SilverEditor(const Silver::ApplicationInfo& inInfo);
	~SilverEditor();

	virtual void OnInit() override;
	virtual void OnShutdown() override;

	virtual void OnUpdate(float inDeltaTime) override;
	virtual void OnEvent(Silver::Event& inEvent) override;
private:
	void DrawUI();
private:
	uint32_t m_CurrentFrame = 0;

	Silver::Ref<Silver::Framebuffer> m_Framebuffer;
	Silver::Ref<Silver::Shader> m_Shader;
	Silver::Ref<Silver::RenderPass> m_RenderPass;
	Silver::Ref<Silver::Pipeline> m_Pipeline;

	Silver::Ref<Silver::VertexBuffer> m_VertexBuffer;
	Silver::Ref<Silver::IndexBuffer> m_IndexBuffer;

	ImGuiLayer m_ImGuiLayer;
	
	Silver::Ref<Silver::Scene> m_ActiveScene = nullptr;
	// std::unique_ptr<PanelManager> m_PanelManeger;
	Silver::OutlinerPanel m_OutlinerPanel;
	Silver::AttributesPanel m_AttributesPanel;

	Silver::Actor m_Actor1;
	Silver::Actor m_Actor2;
};
