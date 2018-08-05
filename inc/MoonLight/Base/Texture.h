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
		enum Bind
		{
			None,
			ShaderResource,		// the texture will be used as a shader resource
			RenderTarget		// we will render on the texture
		};

		bool Create(ml::Window& wnd, ml::Image& img, ml::UInt32 bind = Bind::ShaderResource, bool isCubemap = false, ml::Resource::Usage usg = ml::Resource::Default, ml::UInt32 access = ml::Resource::CPUNone);

		inline ID3D11Texture2D* GetTexture() { return mTexture.Get(); }

	private:
		ml::Ptr<ID3D11Texture2D> mTexture;
	};
}

#endif // __MOONLIGHT_TEXTURE_H__