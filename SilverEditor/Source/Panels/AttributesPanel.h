#pragma once
#include "OutlinerPanel.h"

namespace Silver {

	class AttributesPanel
	{
	public:
		AttributesPanel() = default;

		void OnUIRender();

		void ActorChangedCallback(Actor inNewActor) { m_SelectedActor = inNewActor; }

	private:
		void DrawComponents(Actor inActor);

	private:
		Actor m_SelectedActor;
	};

 }