#pragma once

/* --- CORE --- */
#include "Silver/Core/Application.h"
#include "Silver/Core/Core.h"
#include "Silver/Core/Reference.h"
#include "Silver/Core/Timer.h"
#include "Silver/Core/Logging.h"
#include "Silver/Core/IssueReporting.h"

/* --- SCENE --- */
#include "Silver/Scene/Scene.h"
#include "Silver/Scene/Actor.h"
#include "Silver/Scene/Components.h"

/* --- RENDERER --- */
#include "Silver/Renderer/RendererContext.h"
#include "Silver/Renderer/Swapchain.h"
#include "Silver/Renderer/Shader.h"
#include "Silver/Renderer/RenderPass.h"
#include "Silver/Renderer/Pipeline.h"
#include "Silver/Renderer/Buffer.h"

// EntryPoint.h should be included only by the client and not here
