#pragma once
#include <Silver.h>
#include <Silver/EntryPoint.h>

#include "glm/glm.hpp"
// #include "UI/ImGuiPlatformContext.h"

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
	uint32_t m_CurrentFrame = 0;
	std::vector<Silver::Ref<Silver::CommandBuffer>> m_CommandBuffers;
	Silver::Ref<Silver::CommandBuffer> m_CommandBuffer;

	Silver::Ref<Silver::Shader> m_Shader;
	Silver::Ref<Silver::RenderPass> m_RenderPass;
	Silver::Ref<Silver::Pipeline> m_Pipeline;
};
