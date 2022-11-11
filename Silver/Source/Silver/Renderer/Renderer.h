#pragma once
#include "Silver/Core/Reference.h"

#include "RendererAPI.h"

namespace Silver {

	class Renderer : public RefTarget
	{
	public:
		static RendererAPIType CurrentAPI() { return s_CurrentAPI; }
	private:
		static Ref<Renderer> Create(RendererAPIType InType);
		friend class Application;

		inline static RendererAPIType s_CurrentAPI;
	};

}