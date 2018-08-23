#ifndef __MOONLIGHT_VIEWPORT_H__
#define __MOONLIGHT_VIEWPORT_H__

#include <MoonLight/Base/Math.h>
#include <MoonLight/Base/Window.h>

namespace ml
{
	class Viewport
	{
	public:
		Viewport();
		~Viewport();

		float X[8];
		float Y[8];
		float Width[8];
		float Height[8];

		void Bind(Window& wnd, UInt32 count = 1);

	private:
		std::vector<D3D11_VIEWPORT> m_viewports;
	};
}

#endif // !__MOONLIGHT_VIEWPORT_H__
