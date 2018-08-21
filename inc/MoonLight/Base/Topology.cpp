#include <MoonLight/Base/Topology.h>

ml::UInt32 ml::Topology::ControlPointList(UInt32 count)
{
	assert(count >= 1 && count <= 31);
	return D3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST + (count - 1);
}
