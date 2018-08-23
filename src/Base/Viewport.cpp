#include <MoonLight/Base/Viewport.h>

namespace ml
{
	Viewport::Viewport()
	{
		m_viewports.resize(8);
		for (int i = 0; i < 8; i++) {
			Width[i] = 0;
			Height[i] = 0;
			X[i] = 0;
			Y[i] = 0;
		}
	}
	Viewport::~Viewport()
	{}
	void Viewport::Bind(Window & wnd, UInt32 count)
	{
		for (int i = 0; i < count; i++) {
			m_viewports[i].Width = Width[i];
			m_viewports[i].Height = Height[i];
			m_viewports[i].TopLeftX = X[i];
			m_viewports[i].TopLeftY = Y[i];
			m_viewports[i].MinDepth = 0.0f;
			m_viewports[i].MaxDepth = 1.0f;
		}

		wnd.GetDeviceContext()->RSSetViewports(count, m_viewports.data());
	}
}

