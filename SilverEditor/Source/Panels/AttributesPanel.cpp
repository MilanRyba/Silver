#include "AttributesPanel.h"

#include "Silver/Scene/Components.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace Silver {

	// TODO(Milan): AttributesMenu() - add component...

	void AttributesPanel::OnUIRender()
	{
		ImGui::Begin("Attributes");

		if (m_SelectedActor)
			DrawComponents(m_SelectedActor);

		ImGui::End();
	}

	void AttributesPanel::DrawComponents(Actor inActor)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth 
			| ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		if (inActor.HasComponent<TagComponent>())
		{
			if (ImGui::CollapsingHeader("Tag", flags))
			{
				auto& tag = inActor.GetComponent<TagComponent>().Tag;
				ImGui::Text("Tag"); ImGui::SameLine();

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, tag.c_str());
				if (ImGui::InputText("##", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}
			}
		}

		if (inActor.HasComponent<TransformComponent>())
		{
			if (ImGui::CollapsingHeader("Transform", flags))
			{
				auto& transform = inActor.GetComponent<TransformComponent>().Transform;
				ImGui::Text("Translate"); ImGui::SameLine();
				ImGui::DragFloat3("##", glm::value_ptr(transform[3]), 0.5f);

				size_t hash = typeid(TransformComponent).hash_code();
				AG_TRACE("{0}", hash);
			}
		}
	}

}

