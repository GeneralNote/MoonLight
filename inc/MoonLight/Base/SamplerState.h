#ifndef __MOONLIGHT_SAMPLER_STATE_H__
#define __MOONLIGHT_SAMPLER_STATE_H__
#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Texture.h>

namespace ml
{
	class SamplerState
	{
	public:
		SamplerState();

		bool Create(ml::Window& wnd);

		void BindVS(ml::UInt32 slot = 0);
		void BindGS(ml::UInt32 slot = 0);
		void BindHS(ml::UInt32 slot = 0);
		void BindDS(ml::UInt32 slot = 0);
		void BindPS(ml::UInt32 slot = 0);
		void BindCS(ml::UInt32 slot = 0);

		inline ID3D11SamplerState* GetSampler() { return mSampler.Get(); }

		D3D11_SAMPLER_DESC Info;

	private:
		ml::Ptr<ID3D11SamplerState> mSampler;
		ml::Window* mWnd;
	};
}

#endif //__MOONLIGHT_SAMPLER_STATE_H__