#include <MoonLight/Base/DepthStencilState.h>

namespace ml
{
	DepthStencilState::DepthStencilState()
	{
		Info.DepthEnable = true;
		Info.DepthFunc = D3D11_COMPARISON_LESS;
		Info.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		Info.StencilEnable = FALSE;
		Info.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		Info.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		Info.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		Info.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		Info.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		Info.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		Info.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		Info.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		Info.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		Info.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	}
	bool DepthStencilState::Create(ml::Window & wnd)
	{
		mState.Reset();

		mWnd = &wnd;
		HRESULT hr = wnd.GetDevice()->CreateDepthStencilState(&Info, mState.GetAddressOf());
		return !FAILED(hr);
	}
	void DepthStencilState::Bind(ml::UInt32 stencilRef)
	{
		mWnd->GetDeviceContext()->OMSetDepthStencilState(mState.Get(), stencilRef);
	}
}