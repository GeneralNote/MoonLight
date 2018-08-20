#ifndef __MOONLIGHT_UNORDERED_ACCESS_VIEW_H__
#define __MOONLIGHT_UNORDERED_ACCESS_VIEW_H__
#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Buffer.h>
#include <MoonLight/Base/Texture.h>

namespace ml
{
	class UnorderedAccessView
	{
	public:
		bool Create(ml::Window& wnd, ml::Texture& tex);

		template<typename T>
		bool Create(ml::Window& wnd, ml::Buffer<T>& buffer, DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);

		void Bind(ml::UInt32 slot = 0);

	private:
		ml::Ptr<ID3D11UnorderedAccessView> mView;
		ml::Window* mWnd;
	};

	#include <MoonLight/Base/UnorderedAccessView.inl>
}

#endif //__MOONLIGHT_UNORDERED_ACCESS_VIEW_H__