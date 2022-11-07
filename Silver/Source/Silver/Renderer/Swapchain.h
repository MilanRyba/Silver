#pragma once
#include "Silver/Core/Reference.h"
#include "RendererAPI.h"

struct GLFWwindow;

namespace Silver {

	class Swapchain : public RefTarget
	{
	public:
		virtual ~Swapchain() = default;

		virtual void CreateSurface(GLFWwindow* InWindow) = 0;
		virtual void RecreateSwapchain() = 0;

		virtual uint32_t GetImageCount() = 0;

		static Ref<Swapchain> Create(RendererAPIType InType);
	};

}