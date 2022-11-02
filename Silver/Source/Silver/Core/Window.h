#pragma once
#include <string>
#include <functional>

#include "Silver/Events/Event.h"

struct GLFWwindow;

namespace Silver {

	using EventCallbackFunc = std::function<void(Event&)>;

	struct WindowSpecification
	{
		std::string Title = "";
		uint32_t Width = 0;
		uint32_t Height = 0;
	};

	struct WindowData
	{
		WindowSpecification Specification;
		EventCallbackFunc EventCallback = nullptr;
	};

	class Window
	{
	public:
		Window(const WindowSpecification& spec);
		~Window();

		uint32_t GetWidth() const { return m_WindowData.Specification.Width; }
		uint32_t GetHeight() const { return m_WindowData.Specification.Height; }

		GLFWwindow* GetWindow() const { return m_Window; }

		void SetEventCallback(const EventCallbackFunc& InFunc) { m_WindowData.EventCallback = InFunc; }

		bool ShouldClose() const;
		void Maximize();
	private:
		void Init();
		void ProcessEvents();

		// For Vulkan Instance
		const char** GetRequiredInstanceExtensions(uint32_t* extensionCount);
	private:
		GLFWwindow* m_Window = nullptr;
		WindowData m_WindowData;

		friend class Application;
	};

}