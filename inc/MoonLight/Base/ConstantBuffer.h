#ifndef __MOONLIGHT_CONSTANT_BUFFER_H__
#define __MOONLIGHT_CONSTANT_BUFFER_H__

#include <MoonLight/Base/Buffer.h>
#include <MoonLight/Base/Resource.h>

namespace ml
{
	template <typename T>
	class ConstantBuffer : public Buffer<T>
	{
	public:
		void Create(ml::Window& wnd, T* data, ml::UInt32 dataSize, ml::Resource::Usage usage = ml::Resource::Default, ml::Resource::CPUAccess access = ml::Resource::CPUAccess::CPUNone);
		void Create(ml::Window& wnd, ml::UInt32 dataSize, ml::Resource::Usage = ml::Resource::Default, ml::Resource::CPUAccess = ml::Resource::CPUAccess::CPUNone);
		void Update(T* data);
		void BindVS(ml::UInt32 slot = 0);
		void BindHS(ml::UInt32 slot = 0);
		void BindDS(ml::UInt32 slot = 0);
		void BindGS(ml::UInt32 slot = 0);
		void BindPS(ml::UInt32 slot = 0);
		void BindCS(ml::UInt32 slot = 0);
	};

#include <MoonLight/Base/ConstantBuffer.inl>
}

#endif // __MOONLIGHT_CONSTANT_BUFFER_H__