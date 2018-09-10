#ifndef __MOONLIGHT_GEOMETRY_H__
#define __MOONLIGHT_GEOMETRY_H__

#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/VertexInputLayout.h>

namespace ml
{
	class Geometry
	{
	public:
		struct Vertex
		{
			DirectX::XMFLOAT4 Position;
			DirectX::XMFLOAT2 UV;
			/*
				TODOOOOOOOOOOOOOOOOOOOOOOO
				NORMALS!
			*/
		};

		void Create(ml::Window& wnd, Vertex* verts, size_t count);

		void Draw();

	private:
		ml::Window* mWnd;
		ml::VertexBuffer<Vertex> mVertBuffer;
		size_t mVertCount;
	};
}

#endif // __MOONLIGHT_GEOMETRY_H__