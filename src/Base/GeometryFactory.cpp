#include <MoonLight/Base/GeometryFactory.h>

namespace ml
{
	void generateFace(Geometry::Vertex* verts, float radius, float sx, float sy, int x, int y)
	{
		float phi = y * sy;
		float theta = x * sx;
		verts[2].Position = DirectX::XMFLOAT4(
			(radius*sin(phi)*cos(theta)),
			(radius*sin(phi)*sin(theta)),
			(radius*cos(phi)),
			0
		);
		verts[2].UV = DirectX::XMFLOAT2(theta / DirectX::XM_2PI, phi / DirectX::XM_PI);

		phi = y * sy;
		theta = (x + 1)* sx;
		verts[3].Position = verts[1].Position = DirectX::XMFLOAT4(
			(radius*sin(phi)*cos(theta)),
			(radius*sin(phi)*sin(theta)),
			(radius*cos(phi)),
			0
		);
		verts[3].UV = verts[1].UV = DirectX::XMFLOAT2(theta / DirectX::XM_2PI, phi / DirectX::XM_PI);

		phi = (y + 1) * sy;
		theta = x * sx;
		verts[4].Position = verts[0].Position = DirectX::XMFLOAT4(
			(radius*sin(phi)*cos(theta)),
			(radius*sin(phi)*sin(theta)),
			(radius*cos(phi)),
			0
		);
		verts[4].UV = verts[0].UV = DirectX::XMFLOAT2(theta / DirectX::XM_2PI, phi / DirectX::XM_PI);

		phi = (y + 1) * sy;
		theta = (x + 1) * sx;
		verts[5].Position = DirectX::XMFLOAT4(
			(radius*sin(phi)*cos(theta)),
			(radius*sin(phi)*sin(theta)),
			(radius*cos(phi)),
			0
		);
		verts[5].UV = DirectX::XMFLOAT2(theta / DirectX::XM_2PI, phi / DirectX::XM_PI);
	}

	Geometry GeometryFactory::CreateRectangle(float x, float y, float width, float height, ml::Window& wnd)
	{
		Geometry ret;
		size_t vertCount = 0;

		Geometry::Vertex* verts = GeometryFactory::CreateRectangle(x, y, width, height, vertCount);
		ret.Create(wnd, verts, vertCount);
		delete[] verts;

		return ret;
	}
	Geometry::Vertex* GeometryFactory::CreateRectangle(float x, float y, float width, float height, size_t& count)
	{
		count = 4;
		Geometry::Vertex* ret = new Geometry::Vertex[count];
		
		ret[0].Position = DirectX::XMFLOAT4(x, y, 0, 0);
		ret[1].Position = DirectX::XMFLOAT4(x + width, y, 0, 0);
		ret[2].Position = DirectX::XMFLOAT4(x, y - height, 0, 0);
		ret[3].Position = DirectX::XMFLOAT4(x + width, y - height, 0, 0);

		ret[0].UV = DirectX::XMFLOAT2(0, 0);
		ret[1].UV = DirectX::XMFLOAT2(1, 0);
		ret[2].UV = DirectX::XMFLOAT2(0, 1);
		ret[3].UV = DirectX::XMFLOAT2(1, 1);

		ret[0].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[1].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[2].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[3].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);

		return ret;
	}

	Geometry GeometryFactory::CreateCircle(float x, float y, float radiusX, float radiusY, ml::Window & wnd, size_t pointCount)
	{
		Geometry ret;
		size_t vertCount = 0;

		Geometry::Vertex* verts = GeometryFactory::CreateCircle(x, y, radiusX, radiusY, vertCount, pointCount);
		ret.Create(wnd, verts, vertCount);
		delete[] verts;

		return ret;
	}
	Geometry::Vertex * GeometryFactory::CreateCircle(float x, float y, float radiusX, float radiusY, size_t & count, size_t pointCount)
	{
		count = pointCount * 2 + 1;
		Geometry::Vertex* ret = new Geometry::Vertex[count];

		float step = DirectX::XM_2PI / pointCount;

		ret[0].Position = DirectX::XMFLOAT4(x + radiusX * sin(0), y + radiusY * cos(0), 0, 0);
		ret[1].Position = DirectX::XMFLOAT4(x + radiusX * sin(step), y + radiusY * cos(step), 0, 0);
		ret[2].Position = DirectX::XMFLOAT4(x, y, 0, 0);

		ret[0].UV = DirectX::XMFLOAT2(sin(0) * 0.5f + 0.5f, 1-(cos(0) * 0.5f + 0.5f));
		ret[1].UV = DirectX::XMFLOAT2(sin(step) * 0.5f + 0.5f, 1 - (cos(step) * 0.5f + 0.5f));
		ret[2].UV = DirectX::XMFLOAT2(0.5f, 0.5f);

		ret[0].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[1].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[2].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);

		for (int i = 0; i < pointCount-1; i++) {
			float xVal = sin(step * (i + 2));
			float yVal = cos(step * (i + 2));
			ret[i*2 + 3].Position = DirectX::XMFLOAT4(x + radiusX * xVal, y + radiusY * yVal, 0, 0);
			ret[i*2 + 4].Position = DirectX::XMFLOAT4(x, y, 0, 0);

			ret[i * 2 + 3].UV = DirectX::XMFLOAT2(xVal * 0.5f + 0.5f, 1 - (yVal * 0.5f + 0.5f));
			ret[i * 2 + 4].UV = DirectX::XMFLOAT2(0.5f, 0.5f);

			ret[i * 2 + 3].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
			ret[i * 2 + 4].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		}

		return ret;
	}

	Geometry GeometryFactory::CreateTriangle(float x, float y, float size, ml::Window & wnd)
	{
		Geometry ret;
		size_t vertCount = 0;

		Geometry::Vertex* verts = GeometryFactory::CreateTriangle(x, y, size, vertCount);
		ret.Create(wnd, verts, vertCount);
		delete[] verts;

		return ret;
	}
	Geometry::Vertex * GeometryFactory::CreateTriangle(float x, float y, float size, size_t & count)
	{
		count = 3;
		Geometry::Vertex* ret = new Geometry::Vertex[count];

		float rightOffs = size / tan(DirectX::XMConvertToRadians(30));

		ret[0].Position = DirectX::XMFLOAT4(x, y + size, 0, 0);
		ret[1].Position = DirectX::XMFLOAT4(x + rightOffs, y - size, 0, 0);
		ret[2].Position = DirectX::XMFLOAT4(x - rightOffs, y - size, 0, 0);

		ret[0].UV = DirectX::XMFLOAT2(0.5f, 0);
		ret[1].UV = DirectX::XMFLOAT2(1, 1);
		ret[2].UV = DirectX::XMFLOAT2(0, 1);

		ret[0].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[1].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[2].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);

		return ret;
	}
	
	Geometry GeometryFactory::CreateSphere(float radius, ml::Window & wnd, size_t sliceCount, size_t stackCount)
	{
		Geometry ret;
		size_t vertCount = 0;

		Geometry::Vertex* verts = GeometryFactory::CreateSphere(radius, vertCount, sliceCount, stackCount);
		ret.Create(wnd, verts, vertCount);
		delete[] verts;

		return ret;
	}
	Geometry::Vertex * GeometryFactory::CreateSphere(float radius, size_t & count, size_t sliceCount, size_t stackCount)
	{
		count = sliceCount * stackCount * 6;
		Geometry::Vertex* ret = new Geometry::Vertex[count];

		float stepY = DirectX::XM_PI / stackCount;
		float stepX = DirectX::XM_2PI / sliceCount;

		for (int i = 0; i < stackCount; i++) {
			float phi = i * stepY;
			for (int j = 0; j < sliceCount; j++) {
				float theta = j * stepX;
				size_t index = (i*sliceCount + j) * 6;

				generateFace(ret + index, radius, stepX, stepY, j, i);
			}
		}

		for (int i = 0; i < count; i++)
			DirectX::XMStoreFloat4(&ret[i].Normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat4(&ret[i].Position)));

		return ret;
	}

	Geometry GeometryFactory::CreateCube(float width, float height, float depth, ml::Window & wnd)
	{
		Geometry ret;
		size_t vertCount = 0;

		Geometry::Vertex* verts = GeometryFactory::CreateCube(width, height, depth, vertCount);
		ret.Create(wnd, verts, vertCount);
		delete[] verts;

		return ret;
	}
	Geometry::Vertex * GeometryFactory::CreateCube(float width, float height, float depth, size_t & count)
	{
		count = 36;
		Geometry::Vertex* ret = new Geometry::Vertex[count];

		float halfWidth = width / 2;
		float halfHeight = height / 2;
		float halfDepth = depth / 2;

		// front face
		ret[0].Position = DirectX::XMFLOAT4(-halfWidth, halfHeight, -halfDepth, 0);
		ret[1].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, -halfDepth, 0);
		ret[2].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, -halfDepth, 0);

		ret[3].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, -halfDepth, 0);
		ret[4].Position = DirectX::XMFLOAT4(halfWidth, -halfHeight, -halfDepth, 0);
		ret[5].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, -halfDepth, 0);

		ret[0].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[1].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[2].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[3].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[4].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);
		ret[5].Normal = DirectX::XMFLOAT4(0, 0, -1, 0);

		// back face
		ret[6].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, halfDepth, 0);
		ret[7].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, halfDepth, 0);
		ret[8].Position = DirectX::XMFLOAT4(-halfWidth, halfHeight, halfDepth, 0);

		ret[9].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, halfDepth, 0);
		ret[10].Position = DirectX::XMFLOAT4(halfWidth, -halfHeight, halfDepth, 0);
		ret[11].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, halfDepth, 0);

		ret[6].Normal = DirectX::XMFLOAT4(0, 0, 1, 0);
		ret[7].Normal = DirectX::XMFLOAT4(0, 0, 1, 0);
		ret[8].Normal = DirectX::XMFLOAT4(0, 0, 1, 0);
		ret[9].Normal = DirectX::XMFLOAT4(0, 0, 1, 0);
		ret[10].Normal = DirectX::XMFLOAT4(0, 0, 1, 0);
		ret[11].Normal = DirectX::XMFLOAT4(0, 0, 1, 0);

		// top face
		ret[12].Position = DirectX::XMFLOAT4(-halfWidth, halfHeight, halfDepth, 0);
		ret[13].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, halfDepth, 0);
		ret[14].Position = DirectX::XMFLOAT4(-halfWidth, halfHeight, -halfDepth, 0);

		ret[15].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, halfDepth, 0);
		ret[16].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, -halfDepth, 0);
		ret[17].Position = DirectX::XMFLOAT4(-halfWidth, halfHeight, -halfDepth, 0);

		ret[12].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[13].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[14].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[15].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[16].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[17].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);

		// bottom face
		ret[18].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, -halfDepth, 0);
		ret[19].Position = DirectX::XMFLOAT4(halfWidth, -halfHeight, -halfDepth, 0);
		ret[20].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, halfDepth, 0);

		ret[21].Position = DirectX::XMFLOAT4(halfWidth, -halfHeight, -halfDepth, 0);
		ret[22].Position = DirectX::XMFLOAT4(halfWidth, -halfHeight, halfDepth, 0);
		ret[23].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, halfDepth, 0);

		ret[18].Normal = DirectX::XMFLOAT4(0, -1, 0, 0);
		ret[19].Normal = DirectX::XMFLOAT4(0, -1, 0, 0);
		ret[20].Normal = DirectX::XMFLOAT4(0, -1, 0, 0);
		ret[21].Normal = DirectX::XMFLOAT4(0, -1, 0, 0);
		ret[22].Normal = DirectX::XMFLOAT4(0, -1, 0, 0);
		ret[23].Normal = DirectX::XMFLOAT4(0, -1, 0, 0);

		// right face
		ret[24].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, halfDepth, 0);
		ret[25].Position = DirectX::XMFLOAT4(halfWidth, -halfHeight, -halfDepth, 0);
		ret[26].Position = DirectX::XMFLOAT4(halfWidth, halfHeight,-halfDepth, 0);

		ret[27].Position = DirectX::XMFLOAT4(halfWidth, -halfHeight, -halfDepth, 0);
		ret[28].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, halfDepth, 0);
		ret[29].Position = DirectX::XMFLOAT4(halfWidth, -halfHeight, halfDepth, 0);

		ret[24].Normal = DirectX::XMFLOAT4(1, 0, 0, 0);
		ret[25].Normal = DirectX::XMFLOAT4(1, 0, 0, 0);
		ret[26].Normal = DirectX::XMFLOAT4(1, 0, 0, 0);
		ret[27].Normal = DirectX::XMFLOAT4(1, 0, 0, 0);
		ret[28].Normal = DirectX::XMFLOAT4(1, 0, 0, 0);
		ret[29].Normal = DirectX::XMFLOAT4(1, 0, 0, 0);

		// left face
		ret[30].Position = DirectX::XMFLOAT4(-halfWidth, halfHeight, halfDepth, 0);
		ret[31].Position = DirectX::XMFLOAT4(-halfWidth, halfHeight, -halfDepth, 0);
		ret[32].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, halfDepth, 0);

		ret[33].Position = DirectX::XMFLOAT4(-halfWidth, halfHeight, -halfDepth, 0);
		ret[34].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, -halfDepth, 0);
		ret[35].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, halfDepth, 0);

		ret[30].Normal = DirectX::XMFLOAT4(-1, 0, 0, 0);
		ret[31].Normal = DirectX::XMFLOAT4(-1, 0, 0, 0);
		ret[32].Normal = DirectX::XMFLOAT4(-1, 0, 0, 0);
		ret[33].Normal = DirectX::XMFLOAT4(-1, 0, 0, 0);
		ret[34].Normal = DirectX::XMFLOAT4(-1, 0, 0, 0);
		ret[35].Normal = DirectX::XMFLOAT4(-1, 0, 0, 0);

		// UVs
		ret[0].UV = DirectX::XMFLOAT2(0, 0);
		ret[1].UV = DirectX::XMFLOAT2(1, 0);
		ret[2].UV = DirectX::XMFLOAT2(0, 1);

		ret[3].UV = DirectX::XMFLOAT2(1, 0);
		ret[4].UV = DirectX::XMFLOAT2(1, 1);
		ret[5].UV = DirectX::XMFLOAT2(0, 1);

		ret[6].UV = DirectX::XMFLOAT2(0, 0);
		ret[7].UV = DirectX::XMFLOAT2(1, 1);
		ret[8].UV = DirectX::XMFLOAT2(0, 1);

		ret[9].UV = DirectX::XMFLOAT2(0, 0);
		ret[10].UV = DirectX::XMFLOAT2(1, 0);
		ret[11].UV = DirectX::XMFLOAT2(1, 1);

		ret[12].UV = DirectX::XMFLOAT2(0, 0);
		ret[13].UV = DirectX::XMFLOAT2(1, 0);
		ret[14].UV = DirectX::XMFLOAT2(0, 1);

		ret[15].UV = DirectX::XMFLOAT2(1, 0);
		ret[16].UV = DirectX::XMFLOAT2(1, 1);
		ret[17].UV = DirectX::XMFLOAT2(0, 1);

		ret[18].UV = DirectX::XMFLOAT2(0, 0);
		ret[19].UV = DirectX::XMFLOAT2(1, 0);
		ret[20].UV = DirectX::XMFLOAT2(0, 1);

		ret[21].UV = DirectX::XMFLOAT2(1, 0);
		ret[22].UV = DirectX::XMFLOAT2(1, 1);
		ret[23].UV = DirectX::XMFLOAT2(0, 1);

		ret[24].UV = DirectX::XMFLOAT2(1, 0);
		ret[25].UV = DirectX::XMFLOAT2(0, 1);
		ret[26].UV = DirectX::XMFLOAT2(0, 0);

		ret[27].UV = DirectX::XMFLOAT2(0, 1);
		ret[28].UV = DirectX::XMFLOAT2(1, 0);
		ret[29].UV = DirectX::XMFLOAT2(1, 1);

		ret[30].UV = DirectX::XMFLOAT2(0, 0);
		ret[31].UV = DirectX::XMFLOAT2(1, 0);
		ret[32].UV = DirectX::XMFLOAT2(0, 1);

		ret[33].UV = DirectX::XMFLOAT2(1, 0);
		ret[34].UV = DirectX::XMFLOAT2(1, 1);
		ret[35].UV = DirectX::XMFLOAT2(0, 1);

		return ret;
	}
	Geometry GeometryFactory::CreatePlane(float width, float height, ml::Window & wnd)
	{
		Geometry ret;
		size_t vertCount = 0;

		Geometry::Vertex* verts = GeometryFactory::CreatePlane(width, height, vertCount);
		ret.Create(wnd, verts, vertCount);
		delete[] verts;

		return ret;
	}
	Geometry::Vertex * GeometryFactory::CreatePlane(float width, float height, size_t & count)
	{
		count = 36;
		Geometry::Vertex* ret = new Geometry::Vertex[count];

		float halfWidth = width / 2;
		float halfHeight = height / 2;

		// front face
		ret[0].Position = DirectX::XMFLOAT4(-halfWidth, halfHeight, 0, 0);
		ret[1].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, 0, 0);
		ret[2].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, 0, 0);
		ret[3].Position = DirectX::XMFLOAT4(halfWidth, halfHeight, 0, 0);
		ret[4].Position = DirectX::XMFLOAT4(halfWidth, -halfHeight, 0, 0);
		ret[5].Position = DirectX::XMFLOAT4(-halfWidth, -halfHeight, 0, 0);

		ret[0].UV = DirectX::XMFLOAT2(0, 0);
		ret[1].UV = DirectX::XMFLOAT2(1, 0);
		ret[2].UV = DirectX::XMFLOAT2(0, 1);
		ret[3].UV = DirectX::XMFLOAT2(1, 0);
		ret[4].UV = DirectX::XMFLOAT2(1, 1);
		ret[5].UV = DirectX::XMFLOAT2(0, 1);

		ret[0].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[1].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[2].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[3].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[4].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);
		ret[5].Normal = DirectX::XMFLOAT4(0, 1, 0, 0);

		return ret;
	}
}