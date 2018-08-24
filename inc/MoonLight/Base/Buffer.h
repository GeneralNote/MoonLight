#ifndef __MOONLIGHT_BUFFER_H__
#define __MOONLIGHT_BUFFER_H__

#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/IResource.h>
#include <MoonLight/Base/Ptr.h>

namespace ml
{
	template <typename T>
	class Buffer : public IResource
	{
	public:
		Buffer();
	};

#include <MoonLight/Base/Buffer.inl>
}

#endif // __MOONLIGHT_BUFFER_H__