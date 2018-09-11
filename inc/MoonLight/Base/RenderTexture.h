#ifndef __MOONLIGHT_RENDER_TEXTURE_H__
#define __MOONLIGHT_RENDER_TEXTURE_H__

#include <MoonLight/Base/Resource.h>
#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Ptr.h>
#include <DirectXMath.h>
#include <d3d11.h>

namespace ml
{
	class RenderTexture
	{
	public:
		RenderTexture();

		void Create(ml::Window& wnd, DirectX::XMINT2 size, ml::Resource::Flags flags = 0);
		inline void Clear() { assert(mWnd != nullptr); mWnd->Clear(*this); }
		void Bind();

		inline ID3D11RenderTargetView* GetView() { return mRT.Get(); }
		inline ID3D11Texture2D* GetResource() { return mTexture.Get(); }

	private:
		ml::Window* mWnd;
		ml::Ptr<ID3D11Texture2D> mTexture;
		ml::Ptr<ID3D11RenderTargetView> mRT;
	};
}

#endif // __MOONLIGHT_RENDER_TEXTURE_H__