#pragma once

#include <functional>
#include <map>
#include <unordered_map>
#include <queue>
#include <memory>

#include <glm/glm.hpp>

namespace W_Engine
{
	enum class EventType
	{
		None = 0,
		Event, // used when we just want to handle an arbitrary event 
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMove,
		KeyPress, KeyRelease,
		MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
	};

	using Layer = int;
	constexpr Layer IMGUI_LAYER = 0;
	constexpr Layer ENGINE_LAYER = 1;

	class Event
	{
	public:
		virtual EventType GetEventType() const = 0;
		static EventType GetStaticEventType() { return EventType::Event; }
	};

	class EventDispatcher
	{
	public:
		template <typename T>
		using EventCallback = std::function<bool(const T&)>;

		using EventDispatchFunc = std::function<bool(const Event&)>;
	public:
		EventDispatcher() 
		{
			m_callbacks = std::map<Layer, EventToHandlerMap>{};
		}

		template <typename T>
		void RegisterCallback(EventCallback<T> callback, Layer layer = ENGINE_LAYER)
		{
			m_callbacks[layer][T::GetStaticEventType()] = [callback = std::move(callback)](const Event& event) -> bool
				{
					return callback(static_cast<const T&>(event));
				};
		}

		bool Dispatch(const Event& event)
		{
			EventType eventType = event.GetEventType();

			for (auto& layerAndHandlers: m_callbacks)
			{
				EventToHandlerMap& handlers = layerAndHandlers.second;

				auto it = handlers.find(eventType);
				if (it == handlers.end()) { continue; }

				EventDispatchFunc& handler = it->second;
				if (handler(event)) { return true; }
			}
			return false;
		}
	private:
		using EventToHandlerMap = std::unordered_map<EventType, EventDispatchFunc>;
		std::map<Layer, EventToHandlerMap> m_callbacks;
	};

	class EventQueue
	{
	public:
		EventQueue(EventDispatcher& dispatcher)
			: m_dispatcher(dispatcher)
		{
		}

		void Push(std::unique_ptr<Event> event)
		{
			m_events.push(std::move(event));
		}

		void DispatchEvents()
		{
			while (!m_events.empty())
			{
				std::unique_ptr<Event>& event = m_events.front();
				m_dispatcher.Dispatch(*event.get());
				m_events.pop();
			}
		}
	private:
		std::queue<std::unique_ptr<Event>> m_events;
		EventDispatcher& m_dispatcher;
	};

	// ===============================================
	// TYPES OF EVENTS
	// ===============================================

	class KeyEvent : public Event
	{
	public:
		int GetKeyCode() const { return m_keyCode; }
	protected:
		int m_keyCode;
	protected:
		KeyEvent(int keyCode) : m_keyCode(keyCode) {}
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeats) : KeyEvent(keyCode), m_repeats(repeats) {}
		int GetRepeats() const { return m_repeats; }

		virtual EventType GetEventType() const override { return GetStaticEventType(); }
		static EventType GetStaticEventType() { return EventType::KeyPress; }
	private:
		int m_repeats;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

		virtual EventType GetEventType() const override { return GetStaticEventType(); }
		static EventType GetStaticEventType() { return EventType::KeyRelease; }
	};

	class MouseButtonEvent : public Event
	{
	public:
		int GetMouseButton() const { return m_button; }
	protected:
		int m_button;
	protected:
		MouseButtonEvent(int button) : m_button(button) {}
	};

	class MouseButtonPressEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressEvent(int button)
			: MouseButtonEvent(button)
		{
		}

		virtual EventType GetEventType() const override { return GetStaticEventType(); }
		static EventType GetStaticEventType() { return EventType::MouseButtonPress; }
	};

	class MouseButtonReleaseEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleaseEvent(int button)
			: MouseButtonEvent(button)
		{
		}

		virtual EventType GetEventType() const override { return GetStaticEventType(); }
		static EventType GetStaticEventType() { return EventType::MouseButtonRelease; }
	};

	class MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(float x, float y)
			: m_delta(x, y)
		{
		}

		glm::vec2 GetMove() const { return m_delta; }

		virtual EventType GetEventType() const override { return GetStaticEventType(); }
		static EventType GetStaticEventType() { return EventType::MouseMove; }
	private:
		glm::vec2 m_delta;
	};

	class MouseScrollEvent : public Event
	{
	public:
		MouseScrollEvent(float xOffset, float yOffset)
			: m_delta(xOffset, yOffset)
		{
		}

		glm::vec2 GetScroll() const { return m_delta; }

		virtual EventType GetEventType() const override { return GetStaticEventType(); }
		static EventType GetStaticEventType() { return EventType::MouseScroll; }
	private:
		glm::vec2 m_delta;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		virtual EventType GetEventType() const override { return GetStaticEventType(); }
		static EventType GetStaticEventType() { return EventType::WindowClose; }
	};

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(int width, int height)
			: m_width(width), m_height(height)
		{
		}

		int GetWidth() const { return m_width; }
		int GetHeight() const { return m_height; }

		virtual EventType GetEventType() const override { return GetStaticEventType(); }
		static EventType GetStaticEventType() { return EventType::WindowResize; }
	private:
		int m_width;
		int m_height;
	};
}