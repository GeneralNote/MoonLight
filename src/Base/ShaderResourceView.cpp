#include <MoonLight/Base/ShaderResourceView.h>

namespace ml
{
	bool ShaderResourceView::Create(ml::Window & wnd, ml::Texture & tex)
	{
		HRESULT hr = wnd.GetDevice()->CreateShaderResourceView(tex.GetTexture(), nullptr, mView.GetAddressOf());
		mWnd = &wnd;

		return !FAILED(hr);
	}
	void ShaderResourceView::BindVS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->VSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindGS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->GSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindHS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->HSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindDS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->DSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindPS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->PSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
	void ShaderResourceView::BindCS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->CSSetShaderResources(slot, 1, mView.GetAddressOf());
	}
}