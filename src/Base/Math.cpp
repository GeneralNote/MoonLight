#include <MoonLight/Base/Math.h>

namespace ml
{
	bool operator==(const DirectX::XMINT2 & left, const DirectX::XMINT2 & right)
	{
		return left.x == right.x && left.y == right.y;
	}
	bool operator!=(const DirectX::XMINT2 & left, const DirectX::XMINT2 & right)
	{
		return left.x != right.x || left.y != right.y;
	}
}