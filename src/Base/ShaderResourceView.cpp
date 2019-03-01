#include <MoonLight/Base/ShaderResourceView.h>

namespace ml
{
	bool ShaderResourceView::Create(ml::Window & wnd, ml::Texture & tex)
	{
		mView.Reset();

		HRESULT hr = E_FAIL;

		ID3D11Texture2D* out;
		D3D11_TEXTURE2D_DESC texDesc;
		tex.GetResource()->QueryInterface(IID_ID3D11Texture2D, reinterpret_cast<void**>(&out));
		out->GetDesc(&texDesc);

		if (texDesc.MiscFlags & D3D11_RESOURCE_MISC_TEXTURECUBE) {
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			memset(&desc, 0, sizeof(desc));

			desc.Format = texDesc.Format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
			desc.TextureCube.MostDetailedMip = 0;
			desc.TextureCube.MipLevels = -1;

			hr = wnd.GetDevice()->CreateShaderResourceView(tex.GetResource(), &desc, mView.GetAddressOf());
		} else hr = wnd.GetDevice()->CreateShaderResourceView(tex.GetResource(), nullptr, mView.GetAddressOf());

		mWnd = &wnd;

		return !FAILED(hr);
	}
	bool ShaderResourceView::Create(ml::Window & wnd, ml::RenderTexture & tex)
	{
		mView.Reset();

		HRESULT hr = wnd.GetDevice()->CreateShaderResourceView(tex.GetResource(), nullptr, mView.GetAddressOf());
		mWnd = &wnd;

		return !FAILED(hr);
	}
	void ShaderResourceView::BindVS(ml::UInt32 slot)
	{
		assert(mWnd != nullptr);

		mWnd->GetDeviceContext()->VSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindGS(ml::UInt32 slot)
	{
		assert(mWnd != nullptr);

		mWnd->GetDeviceContext()->GSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindHS(ml::UInt32 slot)
	{
		assert(mWnd != nullptr);

		mWnd->GetDeviceContext()->HSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindDS(ml::UInt32 slot)
	{
		assert(mWnd != nullptr);

		mWnd->GetDeviceContext()->DSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindPS(ml::UInt32 slot)
	{
		assert(mWnd != nullptr);

		mWnd->GetDeviceContext()->PSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindCS(ml::UInt32 slot)
	{
		assert(mWnd != nullptr);

		mWnd->GetDeviceContext()->CSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
}