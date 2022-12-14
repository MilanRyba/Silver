#pragma once
#include "Silver/Core/Reference.h"

#include "RendererContext.h"
#include "Swapchain.h"
#include "CommandBuffer.h"
#include "RenderPass.h"

namespace Silver {

	struct RendererConfig
	{
		uint32_t FramesInFlight;
	};


	class Renderer : public RefTarget
	{
	public:
		static void Init(Ref<RendererContext> inContext, Ref<Swapchain> inSwapchain, RendererConfig inConfig);
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		static void BeginRenderPass(Ref<CommandBuffer> inCommandBuffer, VkRenderPass inRenderPass, uint32_t inImageIndex);
		static void EndRenderPass(Ref<CommandBuffer> inCommandBuffer);

		static RendererConfig GetConfig();
		static Ref<CommandBuffer> GetCurrentCommandBuffer();
	};

}