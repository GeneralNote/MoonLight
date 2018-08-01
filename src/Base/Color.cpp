#include <MoonLight/Base/Color.h>

namespace ml
{
	Color::Color()
	{
		R = G = B = A = 255;
	}
	Color::Color(ml::UInt8 r, ml::UInt8 g, ml::UInt8 b, ml::UInt8 a)
	{
		R = r;
		G = g;
		B = b;
		A = a;
	}
	Color::Color(ml::UInt32 rgba)
	{
		R = rgba & 0xFF000000;
		G = rgba & 0x00FF0000;
		B = rgba & 0x0000FF00;
		A = rgba & 0x000000FF;
	}
	Color::Color(DirectX::FXMVECTOR rgba)
	{
		R = 255 * DirectX::XMVectorGetX(rgba);
		G = 255 * DirectX::XMVectorGetY(rgba);
		B = 255 * DirectX::XMVectorGetZ(rgba);
		A = 255 * DirectX::XMVectorGetW(rgba);
	}
	Color::operator DirectX::XMVECTOR()
	{
		return DirectX::XMVectorSet(R * 1.0f / 255, G * 1.0f / 255, B * 1.0f / 255, A * 1.0f / 255);
	}
}