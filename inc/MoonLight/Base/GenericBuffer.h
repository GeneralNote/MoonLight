#ifndef __MOONLIGHT_GENERIC_BUFFER_H__
#define __MOONLIGHT_GENERIC_BUFFER_H__

#include <MoonLight/Base/Buffer.h>
#include <MoonLight/Base/Resource.h>

namespace ml
{
	template <typename T>
	class GenericBuffer : public Buffer<T>
	{
	public:
		inline void Create(ml::Window& wnd, T* vert, ml::UInt32 vertCount, ml::Resource::Flags flags = 0);
		inline void Create(ml::Window& wnd, ml::UInt32 maxVertCount, ml::Resource::Flags flags = 0);
		inline void Update(T* verts);
	};
	#include <MoonLight/Base/GenericBuffer.inl>
}

#endif // __MOONLIGHT_GENERIC_BUFFER_H__