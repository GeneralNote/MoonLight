#ifndef __MOONLIGHT_VERTEX_BUFFER_H__
#define __MOONLIGHT_VERTEX_BUFFER_H__

#include <MoonLight/Base/Buffer.h>
#include <MoonLight/Base/Resource.h>

namespace ml
{
	template <typename T>
	class VertexBuffer : public Buffer<T>
	{
	public:
		inline void Create(ml::Window& wnd, T* vert, ml::UInt32 vertCount, ml::Resource::Usage usg = ml::Resource::Default, ml::Resource::CPUAccess acc = ml::Resource::CPUAccess::CPUNone);
		inline void Create(ml::Window& wnd, ml::UInt32 maxVertCount, ml::Resource::Usage usg = ml::Resource::Default, ml::Resource::CPUAccess acc = ml::Resource::CPUAccess::CPUNone);
		inline void Update(T* verts);
		inline void Bind(ml::UInt32 slot = 0);
	};
	#include <MoonLight/Base/VertexBuffer.inl>
}

#endif // __MOONLIGHT_VERTEX_BUFFER_H__