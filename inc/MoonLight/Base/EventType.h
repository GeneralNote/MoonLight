#ifndef __MOONLIGHT_EVENT_TYPE_H__
#define __MOONLIGHT_EVENT_TYPE_H__

#include <MoonLight/Base/Math.h>
#include <MoonLight/Base/Keyboard.h>

namespace ml
{
	enum class EventType
	{
		KeyPress,
		KeyRelease,
		TextEnter,
		MouseMove,
		MouseButtonPress,
		MouseButtonRelease,
		Scroll,
		WindowClose,
		WindowResize,
		WindowGainedFocus,
		WindowLostFocus
	};

	typedef DirectX::XMINT2 EventWindowResize;
	typedef DirectX::XMINT2 EventMouseMove;
	typedef ml::UInt8 EventTextEnter;
	struct EventKeyPress
	{
		bool Alt;
		bool Control;
		bool Shift;
		ml::VirtualKey VK;
	};
	struct EventMouseWheel
	{
		float Delta;
		DirectX::XMINT2 Position;
	};
	struct EventMouseButton
	{
		ml::VirtualKey VK;
		DirectX::XMINT2 Position;
	};
}

#endif //__MOONLIGHT_EVENT_TYPE_H__