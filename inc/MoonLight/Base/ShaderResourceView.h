#ifndef __MOONLIGHT_SHADER_RESOURCE_VIEW_H__
#define __MOONLIGHT_SHADER_RESOURCE_VIEW_H__
#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Buffer.h>
#include <MoonLight/Base/Texture.h>

namespace ml
{
	class ShaderResourceView
	{
	public:
		bool Create(ml::Window& wnd, ml::Texture& tex);

		template<typename T>
		bool Create(ml::Window& wnd, ml::Buffer<T>& buffer, DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);

		void BindVS(ml::UInt32 slot = 0);
		void BindGS(ml::UInt32 slot = 0);
		void BindHS(ml::UInt32 slot = 0);
		void BindDS(ml::UInt32 slot = 0);
		void BindPS(ml::UInt32 slot = 0);
		void BindCS(ml::UInt32 slot = 0);

	private:
		ml::Ptr<ID3D11ShaderResourceView> mView;
		ml::Window* mWnd;
	};

	#include <MoonLight/Base/ShaderResourceView.inl>
}

#endif //__MOONLIGHT_SHADER_RESOURCE_VIEW_H__