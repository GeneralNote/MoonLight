#include <MoonLight/Base/Resource.h>
#include <d3d11.h>

#define CHECK_BIT(n, b) ((n & b) == b)

namespace ml
{
	void Resource::Options::Parse(Flags parse, UInt32& usage, UInt32& cpuAccess, UInt32& bind, UInt32& miscFlags)
	{
		switch (parse & 0b11) {
		case 0: usage = D3D11_USAGE_DEFAULT; break;
		case 1: usage = D3D11_USAGE_IMMUTABLE; break;
		case 2: usage = D3D11_USAGE_DYNAMIC; break;
		case 3: usage = D3D11_USAGE_STAGING; break;
		}

		cpuAccess = (CHECK_BIT(parse, CPUAccess::CPUWrite) * D3D11_CPU_ACCESS_WRITE) |
			(CHECK_BIT(parse, CPUAccess::CPURead) * D3D11_CPU_ACCESS_READ);

		miscFlags = ((parse & Flag::GenerateMips) * D3D11_RESOURCE_MISC_GENERATE_MIPS) |
			(CHECK_BIT(parse, Flag::TextureCube) * D3D11_RESOURCE_MISC_TEXTURECUBE) |
			(CHECK_BIT(parse, Flag::RawBufferView) * D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS) |
			(CHECK_BIT(parse, Flag::StructuredBuffer) * D3D11_RESOURCE_MISC_BUFFER_STRUCTURED);

		bind = (CHECK_BIT(parse, Bind::ShaderResource) * D3D11_BIND_SHADER_RESOURCE) |
			(CHECK_BIT(parse, Bind::DepthStencil) * D3D11_BIND_DEPTH_STENCIL) |
			(CHECK_BIT(parse, Bind::UnorderedAccess) * D3D11_BIND_UNORDERED_ACCESS) |
			(CHECK_BIT(parse, Bind::StreamOutput) * D3D11_BIND_STREAM_OUTPUT);
	}
}
