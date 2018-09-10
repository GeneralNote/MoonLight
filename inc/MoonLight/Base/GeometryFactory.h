#ifndef __MOONLIGHT_GEOMETRY_FACTORY_H__
#define __MOONLIGHT_GEOMETRY_FACTORY_H__

#include <MoonLight/Base/Geometry.h>

namespace ml
{
	class GeometryFactory
	{
	public:
		// functions for creating rectangle
		static Geometry CreateRectangle(float x, float y, float width, float height, ml::Window& wnd);
		static Geometry::Vertex* CreateRectangle(float x, float y, float width, float height, size_t& count);

		// functions for creating circle
		static Geometry CreateCircle(float x, float y, float radiusX, float radiusY, ml::Window& wnd, size_t pointCount = 24);
		static Geometry::Vertex* CreateCircle(float x, float y, float radiusX, float radiusY, size_t& count, size_t pointCount = 24);

		// functions for creating triangle
		static Geometry CreateTriangle(float x, float y, float size, ml::Window& wnd);
		static Geometry::Vertex* CreateTriangle(float x, float y, float size, size_t& count);

		// functions for creating sphere
		static Geometry CreateSphere(float radius, ml::Window& wnd, size_t sliceCount = 20, size_t stackCount = 20);
		static Geometry::Vertex* CreateSphere(float radius, size_t& count, size_t sliceCount = 20, size_t stackCount = 20);

		// functions for creating cube
		static Geometry CreateCube(float width, float height, float depth, ml::Window& wnd);
		static Geometry::Vertex* CreateCube(float width, float height, float depth, size_t& count);

		// functions for creating plane
		static Geometry CreatePlane(float width, float height, ml::Window& wnd);
		static Geometry::Vertex* CreatePlane(float width, float height, size_t& count);
	};
}

#endif // __MOONLIGHT_GEOMETRY_FACTORY_H__