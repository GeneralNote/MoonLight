#ifndef __MOONLIGHT_KEYBOARD_H__
#define __MOONLIGHT_KEYBOARD_H__

#include <MoonLight/Base/Math.h>
#include <string>

namespace ml
{
	typedef ml::UInt8 VirtualKey;

	class Keyboard
	{
	public:
		static std::string KeyToString(VirtualKey vk);
	};
}

#endif // __MOONLIGHT_KEYBOARD_H__