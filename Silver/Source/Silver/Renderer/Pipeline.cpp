#include "AgPCH.h"
#include "Pipeline.h"
#include "RendererContext.h"
#include "Silver/Core/Application.h"

namespace Utils {

	VkPrimitiveTopology SilverTopologyToVulkan(Silver::PrimitiveTopology InTopology)
	{
		switch (InTopology)
		{
			case Silver::PrimitiveTopology::None: return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
			case Silver::PrimitiveTopology::Points: return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case Silver::PrimitiveTopology::Lines: return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case Silver::PrimitiveTopology::LineStrip: return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case Silver::PrimitiveTopology::Triangles: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case Silver::PrimitiveTopology::TriangleStrip: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case Silver::PrimitiveTopology::TriangleFan: return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
		}

		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}

}

namespace Silver {

	Pipeline::Pipeline(PipelineInfo info)
	{
		// All of this is pretty much hard-coded (as most other things are) and will be rewritten

		VkPipelineShaderStageCreateInfo vertexShaderInfo{};
		vertexShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderInfo.module = info.Shader->GetShaderModules()[VK_SHADER_STAGE_VERTEX_BIT];
		vertexShaderInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragmentShaderInfo{};
		fragmentShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderInfo.module = info.Shader->GetShaderModules()[VK_SHADER_STAGE_FRAGMENT_BIT];
		fragmentShaderInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderInfos[] = {vertexShaderInfo, fragmentShaderInfo};

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

		VkPipelineInputAssemblyStateCreateInfo assemblyInfo{};
		assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		assemblyInfo.topology = Utils::SilverTopologyToVulkan(info.Topology);
		assemblyInfo.primitiveRestartEnable = VK_FALSE;

		VkExtent2D extent = Application::Get().GetSwapchain()->GetExtent();

		// A viewport basically describes the region of the framebuffer that the output will be rendered to
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)extent.width;
		viewport.height = (float)extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		// Scissor rectangles define in which regions pixels will actually be stored
		VkRect2D scissors{};
		scissors.offset = { 0, 0 };
		scissors.extent = extent;

		// We want viewport and scissors to be dynamic pipeline states
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissors;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = info.LineWidth;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		VkResult layoutResult = vkCreatePipelineLayout(RendererContext::Get().GetDevice(), &pipelineLayoutInfo, nullptr, &m_Layout);
		AG_ASSERT(layoutResult == VK_SUCCESS, "Failed to create Pipeline Layout!");

		VkGraphicsPipelineCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = 2;
		createInfo.pStages = shaderInfos;
		createInfo.pVertexInputState = &vertexInputInfo;
		createInfo.pInputAssemblyState = &assemblyInfo;
		createInfo.pViewportState = &viewportState;
		createInfo.pRasterizationState = &rasterizer;
		createInfo.pMultisampleState = &multisampling;
		createInfo.pDepthStencilState = nullptr; // Optional
		createInfo.pColorBlendState = &colorBlending;
		createInfo.pDynamicState = &dynamicState;
		createInfo.layout = m_Layout;
		createInfo.renderPass = info.RenderPass->GetRenderPass();
		createInfo.subpass = 0;
		createInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		createInfo.basePipelineIndex = -1; // Optional

		VkResult result = vkCreateGraphicsPipelines(RendererContext::Get().GetDevice(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &m_Pipeline);
		AG_ASSERT(result == VK_SUCCESS, "Failed to create Graphics Pipeline!");
		AG_WARN("Created Graphics Pipeline");
	}

	Pipeline::~Pipeline()
	{
		vkDestroyPipelineLayout(RendererContext::Get().GetDevice(), m_Layout, nullptr);
		vkDestroyPipeline(RendererContext::Get().GetDevice(), m_Pipeline, nullptr);
	}

}
