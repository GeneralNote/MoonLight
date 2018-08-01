#include <MoonLight/Base/Math.h>

namespace ml
{
	class Bounds3D
	{
	public:
		Bounds3D();
		Bounds3D(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		Bounds3D(DirectX::XMFLOAT3 mmin, DirectX::XMFLOAT3 mmax);

		DirectX::XMFLOAT3 Min, Max;

		inline float Volume() {
			return (Max.x - Min.x)*(Max.y - Min.y)*(Max.z - Min.z);
		}
		inline DirectX::XMFLOAT3 Center() {
			return DirectX::XMFLOAT3(Max.x - (Max.x - Min.x) / 2.0f, Max.y - (Max.y - Min.y) / 2.0f, Max.z - (Max.z - Min.z) / 2.0f);
		}
	};
}