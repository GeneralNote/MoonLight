#ifndef __MOONLIGHT_TEXTURE_H__
#define __MOONLIGHT_TEXTURE_H__

#include <d3d11.h>
#include <MoonLight/Base/Ptr.h>
#include <MoonLight/Base/Image.h>
#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Resource.h>
#include <MoonLight/Base/IResource.h>

namespace ml
{
	class Texture : public IResource
	{
	public:
		bool Create(ml::Window& wnd, ml::Image& img, ml::Resource::Flags flags = ml::Resource::ShaderResource);
	};
}

#endif // __MOONLIGHT_TEXTURE_H__