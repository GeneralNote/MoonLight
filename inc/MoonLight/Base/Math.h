#ifndef __MOONLIGHT_MATH_H__
#define __MOONLIGHT_MATH_H__
#include <DirectXMath.h>

namespace ml
{
	namespace Math
	{
		inline DirectX::XMVECTOR XMVector2Set(float x, float y) { return DirectX::XMVectorSet(x, y, 0.0f, 0.0f); }
		inline DirectX::XMVECTOR XMVector3Set(float x, float y, float z) { return DirectX::XMVectorSet(x, y, z, 0.0f); }
		inline DirectX::XMFLOAT3 XMFloat3Negate(DirectX::XMFLOAT3 in) { return DirectX::XMFLOAT3(-in.x, -in.y, -in.z); }
	};

	typedef unsigned int UInt32;
	typedef signed int SInt32;
	typedef unsigned short UInt16;
	typedef signed short SInt16;
	typedef unsigned char UInt8;
	typedef signed char SInt8;

	bool operator==(const DirectX::XMINT2& left, const DirectX::XMINT2& right);
	bool operator!=(const DirectX::XMINT2& left, const DirectX::XMINT2& right);
}

#endif // __MOONLIGHT_MATH_H__