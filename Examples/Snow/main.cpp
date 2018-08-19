#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/GeometryShader.h>
#include <MoonLight/Base/ComputeShader.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Base/RasterizerState.h>
#include <MoonLight/Base/DepthStencilState.h>
#include <MoonLight/Base/BlendState.h>
#include <MoonLight/Base/Image.h>
#include <MoonLight/Base/Texture.h>
#include <MoonLight/Base/ShaderResourceView.h>
#include <MoonLight/Base/UnorderedAccessView.h>
#include <MoonLight/Base/SamplerState.h>
#include <MoonLight/Model/OBJModel.h>
#include "../Common/Camera.h"
#include <time.h>

#define THREADS_PER_GROUP 64
#define PARTICLE_COUNT 4096
#define FIELD_HEIGHT 10
#define FIELD_WIDTH 11
#define FIELD_DEPTH 11

struct MyVertex
{
	DirectX::XMFLOAT3 Position;
};

__declspec(align(16))
struct ConstantBufferWVP
{
	DirectX::XMFLOAT4X4 matVP;
	DirectX::XMFLOAT4X4 matWorld;
	DirectX::XMFLOAT3 viewPosition;
};

int main()
{
	srand(time(NULL));

	// create engine
	ml::Window wnd;
	wnd.Create(DirectX::XMINT2(800, 600), "Particles", ml::Window::Style::Resizable);

	// describe our vertex data
	ml::VertexInputLayout vertexInfo;
	vertexInfo.Add("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0);

	// load pixel shader
	ml::PixelShader pxShader;
	pxShader.LoadFromFile(wnd, "pixel.hlsl", "main");

	// load vertex shader + create input signature
	ml::VertexShader vertShader;
	vertShader.InputSignature = &vertexInfo;
	vertShader.LoadFromFile(wnd, "vertex.hlsl", "main");

	// load geometry shader
	ml::GeometryShader geoShader;
	geoShader.LoadFromFile(wnd, "geometry.hlsl", "main");

	// load compute shader
	ml::ComputeShader computeShader;
	computeShader.LoadFromFile(wnd, "compute.hlsl", "main");

	// generate vertices
	std::vector<MyVertex> myVerts(PARTICLE_COUNT);
	for (int i = 0; i < PARTICLE_COUNT; i++)
		myVerts[i].Position = DirectX::XMFLOAT3(
			(((rand() % PARTICLE_COUNT) / (float)PARTICLE_COUNT) * FIELD_WIDTH) - FIELD_WIDTH / 2,
			0,
			(((rand() % PARTICLE_COUNT) / (float)PARTICLE_COUNT) * FIELD_DEPTH) - FIELD_DEPTH / 2
		);

	// create vertex buffer
	ml::VertexBuffer<MyVertex> vertBuffer;
	vertBuffer.Create(wnd, myVerts.data(), PARTICLE_COUNT, ml::Resource::UnorderedAccess | ml::Resource::RawBufferView);

	// create UAV
	ml::UnorderedAccessView vertUAV;
	vertUAV.Create(wnd, vertBuffer);
	vertUAV.Bind();

	// create constant buffer
	ConstantBufferWVP cbWVPData;
	ml::ConstantBuffer<ConstantBufferWVP> cbWVP;
	cbWVP.Create(wnd, &cbWVPData, sizeof(cbWVPData));

	// bind constant buffer
	cbWVP.BindGS();
	
	// camera
	mv::Camera cam;
	cam.SetDistance(20);
	bool mousePressed = false;
	DirectX::XMINT2 mouseContact(0, 0);

	// update constant buffer
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view = cam.GetMatrix();
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);

	DirectX::XMStoreFloat4x4(&cbWVPData.matVP, XMMatrixTranspose(view * proj));
	DirectX::XMStoreFloat4x4(&cbWVPData.matWorld, XMMatrixTranspose(world));
	DirectX::XMStoreFloat3(&cbWVPData.viewPosition, cam.GetPosition());
	cbWVP.Update(&cbWVPData);

	// blend state for rendering billboards/snowflakes
	ml::BlendState alphaBS;
	alphaBS.Info.AlphaToCoverageEnable = true;
	alphaBS.Create(wnd);
	alphaBS.Bind();

	// load snowflake texture
	ml::Image img;
	ml::Texture tex;
	ml::ShaderResourceView srv;

	img.LoadFromFile("snowflake.png");
	tex.Create(wnd, img);
	srv.Create(wnd, tex);
	srv.BindPS();

	// create sampler
	ml::SamplerState smp;
	smp.Create(wnd);
	smp.BindPS();

	ml::RasterizerState rs;
	rs.Info.CullMode = D3D11_CULL_NONE;
	rs.Create(wnd);
	rs.Bind();

	ml::Event e;
	while (wnd.IsOpen()) {
		while (wnd.GetEvent(e)) {
			if (e.Type == ml::EventType::Scroll) {
				// tell the camera to zoom out/in
				cam.Move(-e.MouseWheel.Delta);

				// get the updated matrix
				view = cam.GetMatrix();
			} else if (e.Type == ml::EventType::MouseButtonPress) {
				mousePressed = true;
				mouseContact = e.MouseButton.Position;
			} else if (e.Type == ml::EventType::MouseMove) {
				// if the mouse is pressed and moving aka dragging
				if (mousePressed) {
					// get the delta from the last position
					int dX = e.MouseMove.x - mouseContact.x;
					int dY = e.MouseMove.y - mouseContact.y;

					// save the last position
					mouseContact = e.MouseMove;

					// rotate the camera according to the delta
					cam.RotateX(dX);
					cam.RotateY(dY);

					// get the updated view matrix
					view = cam.GetMatrix();
				}
			} else if (e.Type == ml::EventType::WindowResize) {
				proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);
			} else if (e.Type == ml::EventType::MouseButtonRelease)
				mousePressed = false;
		}

		UINT removeStride = 0;
		UINT reserved = 0;
		ID3D11Buffer* buff = NULL;
		wnd.GetDeviceContext()->IASetVertexBuffers(0, 1, &buff, &removeStride, &reserved);
		vertUAV.Bind();
		computeShader.Bind();
		wnd.Compute(PARTICLE_COUNT / THREADS_PER_GROUP, 1, 1); // the dimensions are not important
		wnd.RemoveUnorderedAccess(0);

		// update the constant buffer every frame
		DirectX::XMStoreFloat4x4(&cbWVPData.matVP, DirectX::XMMatrixTranspose(view * proj));
		DirectX::XMStoreFloat4x4(&cbWVPData.matWorld, DirectX::XMMatrixTranspose(world));
		DirectX::XMStoreFloat3(&cbWVPData.viewPosition, cam.GetPosition());
		cbWVP.Update(&cbWVPData);

		// clear back buffer and depth and stencil buffer
		wnd.Clear();
		wnd.ClearDepthStencil(1.0f, 0);

		// set topology and input signature
		wnd.SetTopology(ml::Topology::PointList);
		wnd.SetInputLayout(vertexInfo);

		// bind the shaders
		vertShader.Bind();
		geoShader.Bind();
		pxShader.Bind();

		// bind the vertex buffer
		vertBuffer.Bind();

		// render the ground
		wnd.Draw(PARTICLE_COUNT);

		// render everything
		wnd.Render();
	}

	wnd.Destroy();

	return 0;
}

