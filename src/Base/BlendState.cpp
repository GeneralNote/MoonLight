#include <MoonLight/Base/BlendState.h>

namespace ml
{
	BlendState::BlendState()
	{
		Info.AlphaToCoverageEnable = false;
		Info.IndependentBlendEnable = false;
		Info.RenderTarget[0].BlendEnable = true;
		Info.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		Info.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Info.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		Info.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		Info.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Info.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		Info.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		SetBlendFactor(ml::Color(0, 0, 0, 0));
	}
	void BlendState::SetBlendFactor(ml::Color color)
	{
		mBlendFactor[0] = color.R * 1.0f / 255;
		mBlendFactor[1] = color.G * 1.0f / 255;
		mBlendFactor[2] = color.B * 1.0f / 255;
		mBlendFactor[3] = color.A * 1.0f / 255;
	}
	bool BlendState::Create(ml::Window & wnd)
	{
		mWnd = &wnd;

		mBlend.Reset();

		HRESULT hr = wnd.GetDevice()->CreateBlendState(&Info, mBlend.GetAddressOf());

		return !FAILED(hr);
	}
	void BlendState::Bind(ml::UInt32 sampleMask)
	{
		assert(mWnd != nullptr);

		mWnd->GetDeviceContext()->OMSetBlendState(mBlend.Get(), mBlendFactor, sampleMask);
	}
}