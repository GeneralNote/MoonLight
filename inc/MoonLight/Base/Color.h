#ifndef __MOONLIGHT_COLOR_H__
#define __MOONLIGHT_COLOR_H__

#include <MoonLight/Base/Math.h>
#include <array>

namespace ml
{
	class Color
	{
	public:
		Color();
		Color(ml::UInt8 r, ml::UInt8 g, ml::UInt8 b, ml::UInt8 a = 255);
		Color(ml::UInt32 rgba);
		Color(DirectX::FXMVECTOR rgba);

		operator DirectX::XMVECTOR();
		operator std::array<float, 4>();

		ml::UInt8 R, G, B, A;
	};
}

#endif // __MOONLIGHT_COLOR_H__
 