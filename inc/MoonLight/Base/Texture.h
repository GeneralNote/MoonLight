#ifndef __MOONLIGHT_TEXTURE_H__
#define __MOONLIGHT_TEXTURE_H__

#include <d3d11.h>
#include <MoonLight/Base/Ptr.h>
#include <MoonLight/Base/Image.h>
#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Resource.h>

namespace ml
{
	class Texture
	{
	public:
		bool Create(ml::Window& wnd, ml::Image& img, ml::Resource::Flags flags = ml::Resource::ShaderResource);

		inline ID3D11Texture2D* GetTexture() { return mTexture.Get(); }

	private:
		ml::Ptr<ID3D11Texture2D> mTexture;
	};
}

#endif // __MOONLIGHT_TEXTURE_H__