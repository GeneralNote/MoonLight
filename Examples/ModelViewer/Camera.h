#pragma once
#include <DirectXMath.h>

namespace mv
{
	class Camera
	{
	public:
		Camera();

		void SetDistance(float d);
		void Move(float d);
		void RotateX(float rx);
		void RotateY(float ry);
		inline float GetDistancce() { return distance; }

		DirectX::XMVECTOR GetPosition();

		DirectX::XMMATRIX GetMatrix();

	private:
		const float maxDistance = 50, minDistance = 2;
		const float maxRotaY = 89;
		float distance;
		float rotaY, rotaX;
	};
}