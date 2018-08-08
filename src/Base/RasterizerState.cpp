#include <MoonLight/Base/RasterizerState.h>

namespace ml
{
	RasterizerState::RasterizerState()
	{
		Info.AntialiasedLineEnable = false;
		Info.CullMode = D3D11_CULL_BACK;
		Info.DepthBias = 0.0f;
		Info.DepthBiasClamp = 0.0f;
		Info.DepthClipEnable = true;
		Info.FillMode = D3D11_FILL_SOLID;
		Info.FrontCounterClockwise = false;
		Info.MultisampleEnable = false;
		Info.ScissorEnable = false;
		Info.SlopeScaledDepthBias = 0.0f;
	}
	bool RasterizerState::Create(ml::Window & wnd)
	{
		mWnd = &wnd;
		
		HRESULT hr = wnd.GetDevice()->CreateRasterizerState(&Info, mRasterizer.GetAddressOf());

		return !FAILED(hr);
	}
	void RasterizerState::Bind()
	{
		mWnd->GetDeviceContext()->RSSetState(mRasterizer.Get());
	}
}