#pragma once
#include "Silver/Core/Reference.h"

#include "CommandBuffer.h"
#include "RenderPass.h"

namespace Silver {

	struct RendererCongfig
	{
		uint32_t FramesInFlight;
	};


	class Renderer : public RefTarget
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginRenderPass(Ref<CommandBuffer> inCommandBuffer, Ref<RenderPass> inRenderPass, uint32_t inImageIndex);
		static void EndRenderPass(Ref<CommandBuffer> inCommandBuffer);

		static void SetConfig(RendererCongfig inConfig);
	};

}