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
		bool Create(ml::Window& wnd, int width, int height, ml::Resource::Flags flags = ml::Resource::ShaderResource, DXGI_FORMAT fmt = DXGI_FORMAT_R8G8B8A8_UNORM);
	};
}

#endif // __MOONLIGHT_TEXTURE_H__