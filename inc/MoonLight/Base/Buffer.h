#ifndef __MOONLIGHT_BUFFER_H__
#define __MOONLIGHT_BUFFER_H__

#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Ptr.h>

namespace ml
{
	template <typename T>
	class Buffer
	{
	public:
		Buffer();

		inline ID3D11Buffer *GetBuffer() { return mBuffer.Get(); }

	protected:
		ml::Ptr<ID3D11Buffer> mBuffer;
		ml::Window* mWindow;
	};

#include <MoonLight/Base/Buffer.inl>
}

#endif // __MOONLIGHT_BUFFER_H__