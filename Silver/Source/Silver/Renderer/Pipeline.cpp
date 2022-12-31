#include "AgPCH.h"
#include "Pipeline.h"
#include "RendererContext.h"
#include "Buffer.h"

#include "Silver/Core/Application.h"

namespace Utils {

	static VkPrimitiveTopology SilverTopologyToVulkan(Silver::PrimitiveTopology InTopology)
	{
		using enum Silver::PrimitiveTopology;
		switch (InTopology)
		{
			case None:			return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
			case Points:		return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case Lines:			return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case LineStrip:		return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case Triangles:		return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case TriangleStrip:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			case TriangleFan:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
		}

		return VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
	}

	static VkFormat ShaderDataTypeToVulkan(Silver::ShaderDataType inType)
	{
		using enum Silver::ShaderDataType;
		switch (inType)
		{
			case Float:		return VK_FORMAT_R32_SFLOAT;
			case Float2:	return VK_FORMAT_R32G32_SFLOAT;
			case Float3:	return VK_FORMAT_R32G32B32_SFLOAT;
			case Float4:	return VK_FORMAT_R32G32B32A32_SFLOAT;
			case Int:		return VK_FORMAT_R32_SINT;
			case Int2:		return VK_FORMAT_R32G32_SINT;
			case Int3:		return VK_FORMAT_R32G32B32_SINT;
			case Int4:		return VK_FORMAT_R32G32B32A32_SINT;
			case Uint:		return VK_FORMAT_R32_UINT;
			case Uint2:		return VK_FORMAT_R32G32_UINT;
			case Uint3:		return VK_FORMAT_R32G32B32_UINT;
			case Uint4:		return VK_FORMAT_R32G32B32A32_UINT;
		}

		return VK_FORMAT_MAX_ENUM;
	}

}

namespace Silver {

	Pipeline::Pipeline(PipelineInfo inInfo)
		: m_Info(inInfo)
	{
		// All of this is pretty much hard-coded (as most other things are) and will be rewritten

		/******************
		*     Shaders     *
		******************/
		VkPipelineShaderStageCreateInfo vertexShaderInfo{};
		vertexShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertexShaderInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertexShaderInfo.module = m_Info.Shader->GetShaderModules()[VK_SHADER_STAGE_VERTEX_BIT];
		vertexShaderInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragmentShaderInfo{};
		fragmentShaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragmentShaderInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragmentShaderInfo.module = m_Info.Shader->GetShaderModules()[VK_SHADER_STAGE_FRAGMENT_BIT];
		fragmentShaderInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderInfos[] = { vertexShaderInfo, fragmentShaderInfo };

		/****************************
		*     Vertex attributes     *
		****************************/
		const std::vector<VertexAttribute>& vertexAttributes = inInfo.Layout.GetAttributes();
		AG_ASSERT(vertexAttributes.size() > 0, "Vertex buffer has no layout");

		VkVertexInputBindingDescription binding = {};
		binding.binding = 0;
		binding.stride = inInfo.Layout.GetStride();
		binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
		attributeDescriptions.reserve(vertexAttributes.size());
		uint32_t location = 0;
		for (const VertexAttribute& attribute : vertexAttributes)
		{
			VkVertexInputAttributeDescription description = {};

			description.binding = binding.binding;
			description.location = location;
			description.offset = attribute.Offset;
			description.format = Utils::ShaderDataTypeToVulkan(attribute.Type);

			attributeDescriptions.emplace_back(description);

			location++;
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &binding;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
		vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

		/*****
		*****/
		VkPipelineInputAssemblyStateCreateInfo assemblyInfo{};
		assemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		assemblyInfo.topology = Utils::SilverTopologyToVulkan(m_Info.Topology);
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
		rasterizer.lineWidth = m_Info.LineWidth;
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

		/*******************
		*     Blending     *
		*******************/
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

		/**************************
		*     Pipeline Layout     *
		**************************/
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
		createInfo.renderPass = m_Info.RenderPass;
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

	void Pipeline::Bind(VkCommandBuffer inCommandBuffer)
	{
		vkCmdBindPipeline(inCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
	}

}
