#include <MoonLight/Base/Bounds.h>

namespace ml
{
	Bounds3D::Bounds3D()
	{
		Min = DirectX::XMFLOAT3(0, 0, 0);
		Max = DirectX::XMFLOAT3(0, 0, 0);
	}
	Bounds3D::Bounds3D(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
	{
		Min = DirectX::XMFLOAT3(minX, minY, minZ);
		Max = DirectX::XMFLOAT3(maxX, maxY, maxZ);
	}
	Bounds3D::Bounds3D(DirectX::XMFLOAT3 mmin, DirectX::XMFLOAT3 mmax)
	{
		Min = mmin;
		Max = mmax;
	}
}