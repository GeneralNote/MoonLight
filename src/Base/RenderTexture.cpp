#include <MoonLight/Base/RenderTexture.h>

namespace ml
{
	RenderTexture::RenderTexture()
	{
		mWnd = nullptr;
	}
	void RenderTexture::Create(ml::Window & wnd, DirectX::XMINT2 size, ml::Resource::Flags flags, bool depthTxt)
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


		// create depth texture if needed
		if (depthTxt) {
			ml::Resource::Options::Parse(flags, (ml::UInt32&)desc.Usage, desc.CPUAccessFlags, desc.BindFlags, desc.MiscFlags);
			desc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

			D3D11_DEPTH_STENCIL_VIEW_DESC* depthDesc = nullptr, actualDepthDesc;
			if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE) {
				desc.Format = DXGI_FORMAT_R24G8_TYPELESS;

				ZeroMemory(&actualDepthDesc, sizeof(actualDepthDesc));
				actualDepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				actualDepthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				if (desc.SampleDesc.Count > 1)
					actualDepthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
				depthDesc = &actualDepthDesc;
			}

			ml::Ptr<ID3D11Texture2D> newDepth;
			wnd.GetDevice()->CreateTexture2D(&desc, nullptr, newDepth.GetAddressOf());
			wnd.GetDevice()->CreateDepthStencilView(newDepth.Get(), depthDesc, mDepth.GetAddressOf());
		}
	}
	void RenderTexture::Bind()
	{
		assert(mWnd != nullptr);
		
		mWnd->GetDeviceContext()->OMSetRenderTargets(1, mRT.GetAddressOf(), (mDepth != nullptr) ? mDepth.Get() : mWnd->GetDepthStencil());
	}
}