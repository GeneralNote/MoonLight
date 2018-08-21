#ifndef __MOONLIGHT_TOPOLOGY_H__
#define __MOONLIGHT_TOPOLOGY_H__
#include <MoonLight/Base/Math.h>
#include <d3d11.h>

namespace ml
{
	class Topology
	{
	public:
		enum Type
		{
			Undefined = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED,
			PointList = D3D_PRIMITIVE_TOPOLOGY_POINTLIST,
			LineList = D3D_PRIMITIVE_TOPOLOGY_LINELIST,
			LineStrip = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP,
			TriangleList = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			TriangleStrip = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
			LineListAdjecent = D3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
			LineStripAdjecent = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
			TriangleListAdjecent = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
			TriangleStripAdjecent = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ
		};

		static UInt32 ControlPointList(UInt32 count);
	};
}

#endif // __MOONLIGHT_TOPOLOGY_H__