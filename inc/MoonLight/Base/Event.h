#ifndef __MOONLIGHT_EVENT_H__
#define __MOONLIGHT_EVENT_H__

#include <MoonLight/Base/EventType.h>

namespace ml
{
	class Event
	{
	public:
		EventType Type;

		EventWindowResize WindowSize;
		EventTextEnter TextCode;
		EventKeyPress Keyboard;
		EventMouseMove MouseMove;
		EventMouseWheel MouseWheel;
		EventMouseButton MouseButton;
	};
}

#endif //__MOONLIGHT_EVENT_H__