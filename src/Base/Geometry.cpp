#include <MoonLight/Base/Geometry.h>

namespace ml
{
	void Geometry::Create(ml::Window& wnd, Vertex * verts, size_t count)
	{
		mWnd = &wnd;
		mVertCount = count;

		mVertBuffer.Create(wnd, verts, count, Resource::Usage::Immutable);
	}
	void Geometry::Draw()
	{
		mVertBuffer.Bind();
		mWnd->Draw(mVertCount);
	}
}