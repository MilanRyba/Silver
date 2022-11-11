#pragma once
#include "Silver/Core/Reference.h"

namespace Silver {

	class RendererContext : public RefTarget
	{
	public:
		virtual void Init(const char** extension, uint32_t extensionCount) = 0;
		virtual void Shutdown() = 0;

		static Ref<RendererContext> Create();
	};

}