#include <MoonLight/Base/SamplerState.h>

namespace ml
{
	SamplerState::SamplerState()
	{
		Info.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		Info.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		Info.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		Info.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		Info.MinLOD = -FLT_MAX;
		Info.MaxLOD = FLT_MAX;
		Info.MipLODBias = 0.0f;
		Info.MaxAnisotropy = 1;
		Info.ComparisonFunc = D3D11_COMPARISON_NEVER;
		Info.BorderColor[0] = Info.BorderColor[1] = Info.BorderColor[2] = Info.BorderColor[3] = 1.0f;
	}
	bool SamplerState::Create(ml::Window& wnd)
	{
		mSampler.Reset();

		HRESULT hr = wnd.GetDevice()->CreateSamplerState(&Info, mSampler.GetAddressOf());
		mWnd = &wnd;

		return !FAILED(hr);
	}
	void SamplerState::BindVS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->VSSetSamplers(slot, 1, mSampler.GetAddressOf());
	}
	void SamplerState::BindGS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->GSSetSamplers(slot, 1, mSampler.GetAddressOf());
	}
	void SamplerState::BindHS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->HSSetSamplers(slot, 1, mSampler.GetAddressOf());
	}
	void SamplerState::BindDS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->DSSetSamplers(slot, 1, mSampler.GetAddressOf());
	}
	void SamplerState::BindPS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->PSSetSamplers(slot, 1, mSampler.GetAddressOf());
	}
	void SamplerState::BindCS(ml::UInt32 slot)
	{
		mWnd->GetDeviceContext()->CSSetSamplers(slot, 1, mSampler.GetAddressOf());
	}
}