#pragma once
#include "Silver/Scene/Scene.h"
#include "Silver/Scene/Actor.h"

namespace Silver {

	using SelectedActorChangedCallbackFunc = std::function<void(Actor&)>;

	class OutlinerPanel
	{
	public:
		OutlinerPanel() = default;
		OutlinerPanel(Ref<Scene> inContext);

		void SetContext(Ref<Scene> inContext);

		void OnUIRender();

		void SetSelectedActorChangedCallback(const SelectedActorChangedCallbackFunc& inFunc) { m_SelectedActorChangedCallback = inFunc; }

	private:
		void DrawActorNode(Actor inActor);
		void SelectedActorChanged(Actor inActor = {});

	private:
		Ref<Scene> m_Context = nullptr;
		Actor m_SelectedActor;

		// Callback that tells Attributes panel that the selected actor has changed
		SelectedActorChangedCallbackFunc  m_SelectedActorChangedCallback;
	};

}