#pragma once
#include "Scene.h"

#include "entt.hpp"

namespace Silver {

	class Actor
	{
	public:
		Actor() = default;
		Actor(const Actor&) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... inArgs)
		{
			AG_ASSERT(!HasComponent<T>(), "Tried to add component that actor already has");
			return m_Scene->m_Registry.emplace<T>(m_ActorHandle, std::forward<Args>(inArgs)...);
		}

		template<typename T>
		T& GetComponent()
		{
			AG_ASSERT(HasComponent<T>(), "Tried to get component that actor does not have");
			return m_Scene->m_Registry.get<T>(m_ActorHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			AG_ASSERT(HasComponent<T>(), "Tried to remove component that actor does not have");
			m_Scene->m_Registry.remove<T>(m_ActorHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			AG_ASSERT(m_ActorHandle != entt::null, "Actor has invalid handle");
			return m_Scene->m_Registry.any_of<T>(m_ActorHandle);
		}

		operator bool() const { return m_ActorHandle != entt::null; }

	private:
		Actor(entt::entity inActorHandle, Scene* inScene) : m_ActorHandle(inActorHandle), m_Scene(inScene) {}

	private:
		entt::entity m_ActorHandle = entt::null;

		/* 
		* Actor should not own the Scene, therefore no Ref<Scene>
		* Ideally we would have like a WeakRef<Scene> and check if the scene is still valid (TODO(Milan))
		*/
		Scene* m_Scene = nullptr;

		friend class Scene;
	};

}