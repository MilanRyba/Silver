#pragma once
#include "Silver/Core/Reference.h"

namespace Silver {

	class Shader : public RefTarget
	{
	public:
		static Ref<Shader> Create(std::string_view InVertex, std::string_view InFragment);
	};

}