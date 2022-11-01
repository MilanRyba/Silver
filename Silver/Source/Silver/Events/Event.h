#pragma once
#include <string>

namespace Silver {

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowMinimize, WindowFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = 1 << 0,
		EventCategoryInput = 1 << 1,
		EventCategoryKeyboard = 1 << 2,
		EventCategoryMouse = 1 << 3,
		EventCategoryMouseButton = 1 << 4
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	class Event
	{
	public:
		bool Handled = false;

		virtual ~Event() {}
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int32_t GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory InCategory) { return InCategory & GetCategoryFlags(); }
	};

	/*	A little how to use...
	*
	*	Construct the dispatcher with the event that came in
	*	If the event in the template of Dispatch() is the same the as the on that came in, it will execute the function in the parameter
	*		- The parameter is a function that also takes in the event in the template
	*		- It needs to return bool (preferably, otherwise the compiler will yell at you)
	*
	*	EventDispatcher dispacther(InEvent);
	*	dispacther.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& InPressedEvent)
	*	{
	*		AG_CORE_WARN("{0}", InEvent);
	*		return true;
	*	});
	*
	*/

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& InEvent)
			: m_Event(InEvent) {}

		template<typename Event, typename EventFunc>
		void Dispatch(const EventFunc& InEventFunc)
		{
			if (m_Event.GetEventType() == Event::GetStaticType())
				m_Event.Handled |= InEventFunc(static_cast<Event&>(m_Event));
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& InOutstream, const Event& InEvent)
	{
		return InOutstream << InEvent.ToString();
	}

}