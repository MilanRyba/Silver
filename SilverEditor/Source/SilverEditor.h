#pragma once
#include <Silver.h>
#include <Silver/EntryPoint.h>

#include "glm/glm.hpp"
// #include "UI/ImGuiPlatformContext.h"

class SilverEditor : public Silver::Application
{
public:
	SilverEditor(const Silver::ApplicationSpecification& InSpecification);
	~SilverEditor();

	virtual void OnInit() override;
	virtual void OnShutdown() override;

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnEvent(Silver::Event& InEvent) override;
private:
	Silver::Ref<Silver::Shader> m_Shader;
};
