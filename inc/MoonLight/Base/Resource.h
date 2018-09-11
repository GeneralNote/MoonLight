#ifndef __MOONLIGHT_RESOURCE_H__
#define __MOONLIGHT_RESOURCE_H__

#include <MoonLight/Base/Math.h>

namespace ml
{
	class Resource
	{
	public:
		typedef ml::UInt16 Flags;

		// 0000 0000 0000 0000
		//                  ^^
		enum Usage
		{
			Default		= 0,
			Immutable	= 0b01,
			Dynamic		= 0b10,
			Staging		= 0b11
		};

		// 0000 0000 0000 0000
		//                ^^
		enum CPUAccess
		{
			NoCPU		= 0,
			CPUWrite	= 0b01 << 2,
			CPURead		= 0b10 << 2
		};

		// 0000 0000 0000 0000
		//           ^^^^
		enum Flag
		{
			NoFlags				= 0,
			GenerateMips		= 0b0001 << 4,
			TextureCube			= 0b0010 << 4,
			RawBufferView		= 0b0100 << 4,
			StructuredBuffer	= 0b1000 << 4
		};

		// 0000 0000 0000 0000
		//    ^ ^^^^
		enum Bind
		{
			NoBind				= 0,
			ShaderResource		= 0b00001 << 8, // the resource will be used in a shader as a read only
			DepthStencil		= 0b00010 << 8, // the texture will be used as a depth stencil texture
			//Reserved			= 0b00100 << 8, // NOTE: this was supposed to be the RenderTarget flag but we dont need it
			UnorderedAccess		= 0b01000 << 8, // the resource will be used as a RW resource in compute shader
			StreamOutput		= 0b10000 << 8  // not implemented yet
		};


		/*
			This class is used for generating and reading D3D11 flags
			from two bytes.
		*/
		struct Options
		{
			// convert to d3d11 equivalent values
			static void Parse(Flags parse, UInt32& usage, UInt32& cpuAccess, UInt32& bind, UInt32& miscFlags);
		};
	};
}

#endif // __MOONLIGHT_RESOURCE_H__