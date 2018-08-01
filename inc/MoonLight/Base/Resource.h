#ifndef __MOONLIGHT_RESOURCE_H__
#define __MOONLIGHT_RESOURCE_H__

#include <d3d11.h>

namespace ml
{
	class Resource
	{
	public:
		enum Usage
		{
			Default = D3D11_USAGE_DEFAULT,
			Immutable = D3D11_USAGE_IMMUTABLE,
			Dynamic = D3D11_USAGE_DYNAMIC,
			Staging = D3D11_USAGE_STAGING
		};
		enum CPUAccess
		{
			CPUNone = 0,
			CPUWrite = D3D11_CPU_ACCESS_WRITE,
			CPURead = D3D11_CPU_ACCESS_READ
		};
	};
}

#endif // __MOONLIGHT_RESOURCE_H__