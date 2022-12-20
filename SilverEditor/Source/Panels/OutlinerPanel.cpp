#include "OutlinerPanel.h"
#include "Silver/Scene/Components.h"

#include <imgui.h>

namespace Silver {

	OutlinerPanel::OutlinerPanel(Ref<Scene> inContext)
		: m_Context(inContext) {}

	void OutlinerPanel::SetContext(Ref<Scene> inContext)
	{
		m_Context = inContext;
	}

	void OutlinerPanel::OnUIRender()
	{
		ImGui::Begin("Outliner");

		m_Context->m_Registry.each([&](auto actorHandle)
		{
			Actor actor = { actorHandle, m_Context.GetPtr() };
			DrawActorNode(actor);
		});

		// Deselecting actors in the outliner panel
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered())
			SelectedActorChanged();

		ImGui::End();

		// TODO(Milan): Remove
		ImGui::ShowDemoWindow();
	}

	void OutlinerPanel::DrawActorNode(Actor inActor)
	{
		auto& tag = inActor.GetComponent<TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		flags |= (m_SelectedActor == inActor ? ImGuiTreeNodeFlags_Selected : 0);

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)inActor, flags, tag.c_str());

		// If this actor was selected (clicked)
		if (ImGui::IsItemClicked())
			SelectedActorChanged(inActor);

		if (opened)
		{
			ImGui::Text("%s", tag.c_str());
			ImGui::TreePop();
		}
	}

	void OutlinerPanel::SelectedActorChanged(Actor inActor)
	{
		m_SelectedActor = inActor;
		m_SelectedActorChangedCallback(inActor);
	}

}