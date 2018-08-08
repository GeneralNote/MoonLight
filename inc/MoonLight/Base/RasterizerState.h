#ifndef __MOONLIGHT_RASTERIZER_STATE_H__
#define __MOONLIGHT_RASTERIZER_STATE_H__

#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Ptr.h>
#include <d3d11.h>

namespace ml
{
	class RasterizerState
	{
	public:
		RasterizerState();

		D3D11_RASTERIZER_DESC Info;

		bool Create(ml::Window& wnd);

		void Bind();

	private:
		ml::Ptr<ID3D11RasterizerState> mRasterizer;
		ml::Window* mWnd;
	};
}

#endif // __MOONLIGHT_RASTERIZER_STATE_H__