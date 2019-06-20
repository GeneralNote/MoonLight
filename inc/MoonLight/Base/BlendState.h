#ifndef __MOONLIGHT_BLEND_STATE_H__
#define __MOONLIGHT_BLEND_STATE_H__

#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Ptr.h>
#include <d3d11.h>

namespace ml
{
	class BlendState
	{
	public:
		BlendState();

		D3D11_BLEND_DESC Info;

		inline ml::Color GetBlendFactor() { return ml::Color(255 * mBlendFactor[0], 255 * mBlendFactor[1], 255 * mBlendFactor[2], 255 * mBlendFactor[3]); }
		void SetBlendFactor(ml::Color clr);

		bool Create(ml::Window& wnd);

		void Bind(ml::UInt32 sampleMask = 0xFFFFFFFF);

		inline ID3D11BlendState* GetBlend() { return mBlend.Get(); }

	private:
		ml::Ptr<ID3D11BlendState> mBlend;
		ml::Window* mWnd;
		float mBlendFactor[4];
	};
}

#endif // __MOONLIGHT_BLEND_STATE_H__