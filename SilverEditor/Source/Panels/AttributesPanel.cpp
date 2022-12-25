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
			auto& transform = inActor.GetComponent<TransformComponent>().Transform;

			if (ImGui::CollapsingHeader("Transform_Header", flags))
			{
				ImGuiTableFlags flags = ImGuiTableFlags_Resizable;
				if (ImGui::BeginTable("Transform_Table", 2, flags))
				{
					ImGui::TableNextColumn(); ImGui::Text("Translate");
					ImGui::Separator();
					ImGui::TableNextColumn(); ImGui::DragFloat3("##", glm::value_ptr(transform[3]), 0.1f);

					ImGui::TableNextColumn(); ImGui::Text("Rotate");
					ImGui::Separator();
					ImGui::TableNextColumn(); ImGui::DragFloat3("##", glm::value_ptr(transform[3]), 0.1f);

					ImGui::TableNextColumn(); ImGui::Text("Scale");
					ImGui::Separator();
					ImGui::TableNextColumn(); ImGui::DragFloat3("##", glm::value_ptr(transform[3]), 0.1f);

					ImGui::EndTable();
				}

				// size_t hash = typeid(TransformComponent).hash_code();
				// AG_CRITICAL("{0}", hash);
			}
		}
	}

}

