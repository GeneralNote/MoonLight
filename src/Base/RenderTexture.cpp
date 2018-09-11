#include <MoonLight/Base/RenderTexture.h>

namespace ml
{
	RenderTexture::RenderTexture()
	{
		mWnd = nullptr;
	}
	void RenderTexture::Create(ml::Window & wnd, DirectX::XMINT2 size, ml::Resource::Flags flags)
	{
		assert(size.x > 0);
		assert(size.y > 0);

		mTexture.Reset();
		mRT.Reset();
		mWnd = &wnd;

		// get depth stencil texture info
		ID3D11Texture2D* depthTexture;
		D3D11_TEXTURE2D_DESC depthDesc;
		wnd.GetDepthStencil()->GetResource(reinterpret_cast<ID3D11Resource**>(&depthTexture));
		depthTexture->GetDesc(&depthDesc);
		depthTexture->Release();

		// create 2D texture
		D3D11_TEXTURE2D_DESC desc;
		ml::Resource::Options::Parse(flags, (ml::UInt32&)desc.Usage, desc.CPUAccessFlags, desc.BindFlags, desc.MiscFlags);

		desc.ArraySize = 1;
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.Height = size.y;
		desc.MipLevels = 1;
		desc.Width = size.x;
		desc.SampleDesc = depthDesc.SampleDesc;

		wnd.GetDevice()->CreateTexture2D(&desc, nullptr, mTexture.GetAddressOf());

		// create render target view
		wnd.GetDevice()->CreateRenderTargetView(mTexture.Get(), nullptr, mRT.GetAddressOf());
	}
	void RenderTexture::Bind()
	{
		assert(mWnd != nullptr);

		mWnd->GetDeviceContext()->OMSetRenderTargets(1, mRT.GetAddressOf(), mWnd->GetDepthStencil());
	}
}