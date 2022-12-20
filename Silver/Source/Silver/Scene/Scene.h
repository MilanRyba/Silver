#pragma once
#include "Silver/Core/Reference.h"

// #include "Panels/OutlinerPanel.h"

#include "entt.hpp"

namespace Silver {

	class Actor;

	class Scene : public RefTarget
	{
	public:
		Scene();
		~Scene();

		Actor CreateActor(std::string_view inTag = "");

		void OnUpdate(float inDeltaTime);

	private:
		entt::registry m_Registry;

		friend class Actor;
		friend class OutlinerPanel;
	};

}