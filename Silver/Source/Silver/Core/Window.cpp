#include "AgPCH.h"
#include "Window.h"
#include "Silver/Core/IssueReporting.h"

#include "Silver/Events/ApplicationEvents.h"
#include "Silver/Events/KeyEvents.h"
#include "Silver/Events/MouseEvents.h"

#include <GLFW/glfw3.h>

namespace Silver {

	Window::Window(const WindowSpecification& InSpecification)
	{
		m_WindowData.Specification = InSpecification;
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void Window::Init()
	{
		int32_t success = glfwInit();
		AG_ASSERT(success, "Failed to initialize GLFW!");

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow((int)m_WindowData.Specification.Width, (int)m_WindowData.Specification.Height, m_WindowData.Specification.Title.c_str(), nullptr, nullptr);

		AG_ASSERT(m_Window, "Failed to create GLFW Window!");
		AG_CORE_INFO("Created Window!");

		int32_t vulkan = glfwVulkanSupported();
		AG_ASSERT(success, "Vulkan is not supported!");

		glfwSetWindowUserPointer(m_Window, &m_WindowData);

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* InWindow, int InWidth, int InHeight)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(InWindow));

			data->Specification.Width = InWidth;
			data->Specification.Height = InHeight;

			WindowResizeEvent resizeEvent(InWidth, InHeight);
			data->EventCallback(resizeEvent);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* InWindow)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(InWindow));
			WindowCloseEvent closeEvent;
			data->EventCallback(closeEvent);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* InWindow, int InKey, int InScanCode, int InAction, int InMods)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(InWindow));

			switch (InAction)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent keyEvent(InKey);
					data->EventCallback(keyEvent);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent keyEvent(InKey);
					data->EventCallback(keyEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent keyEvent(InKey);
					data->EventCallback(keyEvent);
					break;
				}
			}
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* InWindow, int InButton, int InAction, int InMods)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(InWindow));

			switch (InAction)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent pressedEvent(InButton);
					data->EventCallback(pressedEvent);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent releasedEvent(InButton);
					data->EventCallback(releasedEvent);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* InWindow, double InOffsetX, double InOffsetY)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(InWindow));

			MouseScrolledEvent event((float)InOffsetX, (float)InOffsetY);
			data->EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* InWindow, double InPosX, double InPosY)
		{
			WindowData* data = static_cast<WindowData*>(glfwGetWindowUserPointer(InWindow));

			MouseMovedEvent event((float)InPosX, (float)InPosY);
			data->EventCallback(event);
		});
	}

	void Window::ProcessEvents()
	{
		glfwPollEvents();
	}


	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::Maximize()
	{
		glfwMaximizeWindow(m_Window);
	}

	const char** Window::GetRequiredInstanceExtensions(uint32_t* InExtensionCount)
	{
		const char** extensions = glfwGetRequiredInstanceExtensions(InExtensionCount);
		return extensions;
	}
}