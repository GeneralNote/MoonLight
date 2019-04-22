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

		void Create(ml::Window& wnd, DirectX::XMINT2 size, ml::Resource::Flags flags = 0, bool createDepthTxt = false, int bpp = 8);
		inline void Clear() { assert(mWnd != nullptr); mWnd->Clear(*this); }
		inline void ClearDepthStencil(float depth, ml::UInt8 stencil) { assert(mWnd != nullptr); mWnd->ClearDepthStencil(*this, depth, stencil); }
		inline void ClearStencil(ml::UInt8 value) { assert(mWnd != nullptr); mWnd->ClearStencil(*this, value); }

		void Bind();

		inline ID3D11RenderTargetView* GetView() { return mRT.Get(); }
		inline ID3D11DepthStencilView* GetDepthStencilView() { return mDepth.Get(); }
		inline ID3D11Texture2D* GetResource() { return mTexture.Get(); }

	private:
		ml::Window* mWnd;
		ml::Ptr<ID3D11Texture2D> mTexture;
		ml::Ptr<ID3D11RenderTargetView> mRT;
		ml::Ptr<ID3D11DepthStencilView> mDepth;
	};
}

#endif // __MOONLIGHT_RENDER_TEXTURE_H__