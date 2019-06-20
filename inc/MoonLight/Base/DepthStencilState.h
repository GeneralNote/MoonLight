#ifndef __MOONLIGHT_DEPTH_STENCIL_STATE_H__
#define __MOONLIGHT_DEPTH_STENCIL_STATE_H__

#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Ptr.h>
#include <d3d11.h>

namespace ml
{
	class DepthStencilState
	{
	public:
		DepthStencilState();

		D3D11_DEPTH_STENCIL_DESC Info;

		bool Create(ml::Window& wnd);

		void Bind(ml::UInt32 stencilRef = 0xFFFFFFFF);

		inline ID3D11DepthStencilState* GetState() { return mState.Get(); }

	private:
		ml::Ptr<ID3D11DepthStencilState> mState;
		ml::Window* mWnd;
	};
}

#endif // __MOONLIGHT_BLEND_STATE_H__