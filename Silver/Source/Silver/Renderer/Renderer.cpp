#include "AgPCH.h"
#include "Renderer.h"

#include "Silver/Core/Application.h"

namespace Silver {

	struct RendererData
	{
		Ref<RendererContext> Context = nullptr;
		Ref<Swapchain> Swapchain = nullptr;

		uint32_t CurrentFrame = 0;

		std::vector<VkSemaphore> ImageAvailableSemaphores;
		std::vector<VkSemaphore> RenderFinishedSemaphores;
		std::vector<VkFence> ImageFences;

		std::vector<Ref<CommandBuffer>> CommandBuffers;
	};

	static RendererData* s_Data = nullptr;
	static RendererConfig s_Config;

	void Renderer::Init(Ref<RendererContext> inContext, Ref<Swapchain> inSwapchain, RendererConfig inConfig)
	{
		s_Config = inConfig;
		s_Data = new RendererData;

		s_Data->Context = inContext;
		s_Data->Swapchain = inSwapchain;

		{
			s_Data->ImageAvailableSemaphores.resize(s_Config.FramesInFlight);
			s_Data->RenderFinishedSemaphores.resize(s_Config.FramesInFlight);
			s_Data->ImageFences.resize(s_Config.FramesInFlight);

			VkSemaphoreCreateInfo semaphoreInfo{};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			VkFenceCreateInfo fenceInfo{};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			for (uint32_t i = 0; i < s_Config.FramesInFlight; i++)
			{
				vkCreateSemaphore(s_Data->Context->Get().GetDevice(), &semaphoreInfo, nullptr, &s_Data->ImageAvailableSemaphores[i]);
				vkCreateSemaphore(s_Data->Context->Get().GetDevice(), &semaphoreInfo, nullptr, &s_Data->RenderFinishedSemaphores[i]);
				vkCreateFence(s_Data->Context->Get().GetDevice(), &fenceInfo, nullptr, &s_Data->ImageFences[i]);
			}

			s_Data->CommandBuffers.resize(s_Config.FramesInFlight);
			for (auto& buffer : s_Data->CommandBuffers)
			{
				VkCommandBuffer cmdBuffer = s_Data->Context->Get().CreatePrimaryCommandBuffer();
				buffer = new CommandBuffer(cmdBuffer);
			}
		}
	}

	void Renderer::Shutdown()
	{
		for (uint32_t i = 0; i < Renderer::GetConfig().FramesInFlight; i++)
		{
			vkDestroySemaphore(s_Data->Context->Get().GetDevice(), s_Data->ImageAvailableSemaphores[i], nullptr);
			vkDestroySemaphore(s_Data->Context->Get().GetDevice(), s_Data->RenderFinishedSemaphores[i], nullptr);
			vkDestroyFence(s_Data->Context->Get().GetDevice(), s_Data->ImageFences[i], nullptr);
		}

		delete s_Data;
		s_Data = nullptr;
	}

	void Renderer::BeginFrame()
	{
		VkDevice device = s_Data->Context->Get().GetDevice();

		// Waits for the previous frame to finish
		vkWaitForFences(device, 1, &s_Data->ImageFences[s_Data->CurrentFrame], VK_TRUE, UINT64_MAX);

		// Acquires next image from swapchain
		s_Data->Swapchain->AcquireNextImage(s_Data->ImageAvailableSemaphores[s_Data->CurrentFrame]);
		
		vkResetFences(device, 1, &s_Data->ImageFences[s_Data->CurrentFrame]);

		s_Data->CommandBuffers[s_Data->CurrentFrame]->Reset();
		s_Data->CommandBuffers[s_Data->CurrentFrame]->Begin();
	}

	void Renderer::EndFrame()
	{
		s_Data->CommandBuffers[s_Data->CurrentFrame]->End();

		{
			VkSemaphore waitSemaphores[] = { s_Data->ImageAvailableSemaphores[s_Data->CurrentFrame] };
			VkPipelineStageFlags waitStageFlags[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
			VkSemaphore signalSemaphores[] = { s_Data->RenderFinishedSemaphores[s_Data->CurrentFrame] };

			VkCommandBuffer commandBuffer = s_Data->CommandBuffers[s_Data->CurrentFrame]->GetCommandBuffer();

			VkSubmitInfo submitInfo = {};
			submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = waitSemaphores;
			submitInfo.pWaitDstStageMask = waitStageFlags;
			submitInfo.commandBufferCount = 1;
			submitInfo.pCommandBuffers = &commandBuffer;

			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = signalSemaphores;

			const auto& QueueHandle = s_Data->Context->GetGraphicsQueue().QueueHandle;
			VkResult result = vkQueueSubmit(QueueHandle, 1, &submitInfo, s_Data->ImageFences[s_Data->CurrentFrame]);
		}

		s_Data->Swapchain->Present(s_Data->RenderFinishedSemaphores[s_Data->CurrentFrame]);

		s_Data->CurrentFrame = (s_Data->CurrentFrame + 1) % s_Config.FramesInFlight;
	}

	void Renderer::BeginRenderPass(Ref<CommandBuffer> inCommandBuffer, VkRenderPass inRenderPass, uint32_t inImageIndex)
	{
		VkRenderPassBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		beginInfo.renderPass = inRenderPass;
		beginInfo.framebuffer = s_Data->Swapchain->GetCurrentFramebuffer();
		beginInfo.renderArea.offset = { 0, 0 };
		beginInfo.renderArea.extent = s_Data->Swapchain->GetExtent();

		VkClearValue clearValue = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
		beginInfo.pClearValues = &clearValue;
		beginInfo.clearValueCount = 1;

		vkCmdBeginRenderPass(inCommandBuffer->GetCommandBuffer(), &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void Renderer::EndRenderPass(Ref<CommandBuffer> inCommandBuffer)
	{
		vkCmdEndRenderPass(inCommandBuffer->GetCommandBuffer());
	}

	RendererConfig Renderer::GetConfig() { return s_Config; }

	Ref<CommandBuffer> Renderer::GetCurrentCommandBuffer() { return s_Data->CommandBuffers[s_Data->CurrentFrame]; }

}