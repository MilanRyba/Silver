#include "AgPCH.h"
#include "Scene.h"
#include "Components.h"
#include "Actor.h"

#include <glm/glm.hpp>

namespace Silver {

	Scene::Scene()
	{
#ifdef ENTT_EXAMPLE
		// Creates entity and gives us its id
		entt::entity entity = m_Registry.create();

		// Adds a component to an entity
		m_Registry.emplace<PointLightComponent>(entity);

		// Retrieves a reference to the component
		auto& pointLight = m_Registry.get<PointLightComponent>(entity);

		// ... if the entity has the component
		if (m_Registry.any_of<PointLightComponent>(entity))
			pointLight = m_Registry.get<PointLightComponent>(entity);

		// Retrieves all of the entities with that component
		auto view = m_Registry.view<PointLightComponent>();
		for (auto entity : view)
		{
			auto& pointLight = view.get<PointLightComponent>(entity);
		}

		// auto group = m_Registry.group<PointLightComponent>(entt::get<TransformComponent>);
		// for (auto entity : group)
		// {
		// 	const auto&[pointLight, transform] = group.get<PointLightComponent, TransformComponent>(entity);
		// }
#endif
	}

	Scene::~Scene()
	{
	}

	Actor Scene::CreateActor(std::string_view inTag)
	{
		Actor actor = { m_Registry.create(), this };

		inTag = inTag.empty() ? "Untitled Actor" : inTag;
		auto& tag = actor.AddComponent<TagComponent>(inTag);

		actor.AddComponent<TransformComponent>();

		return actor;
	}

	void Scene::OnUpdate(float inDeltaTime)
	{

	}

}