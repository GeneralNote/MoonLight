#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/GeometryShader.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Base/RasterizerState.h>
#include <MoonLight/Base/DepthStencilState.h>
#include <MoonLight/Base/BlendState.h>
#include <MoonLight/Base/Image.h>
#include <MoonLight/Base/Texture.h>
#include <MoonLight/Base/ShaderResourceView.h>
#include <MoonLight/Base/SamplerState.h>
#include <MoonLight/Model/OBJModel.h>
#include "../Common/Camera.h"

struct MyVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};

__declspec(align(16))
struct ConstantBufferWVP
{
	DirectX::XMFLOAT4X4 matVP;
	DirectX::XMFLOAT4X4 matWorld;
	float ApplyVP;
};

__declspec(align(16))
struct ConstantBufferCamera
{
	DirectX::XMFLOAT3 viewPos;
};

int main()
{
	// create engine
	ml::Window wnd;
	wnd.Create(DirectX::XMINT2(800, 600), "Billboard", ml::Window::Style::Resizable);

	// describe our vertex data
	ml::VertexInputLayout vertexInfo;
	vertexInfo.Add("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0);
	vertexInfo.Add("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 12);

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

	// load model
	ml::OBJModel obj;
	obj.LoadFromFile("elements.obj");

	// create vertex buffer
	ml::VertexBuffer<ml::OBJModel::Vertex> vertBuffer;
	vertBuffer.Create(wnd, obj.GetVertexData(), obj.GetVertexCount(), ml::Resource::Immutable);

	// create constant buffer
	ConstantBufferWVP cbDataWVP;
	ml::ConstantBuffer<ConstantBufferWVP> cbWVP;
	cbWVP.Create(wnd, &cbDataWVP, sizeof(cbDataWVP));

	// bind constant buffer
	cbWVP.BindVS();
	cbWVP.BindGS();
	cbWVP.BindPS();

	// camera
	mv::Camera cam;
	cam.SetDistance(10);
	cam.RotateY(-80);
	cam.RotateX(-90);
	bool mousePressed = false;
	DirectX::XMINT2 mouseContact(0, 0);

	// create constant buffer for camera
	ConstantBufferCamera cbCameraData;
	ml::ConstantBuffer<ConstantBufferCamera> cbCamera;
	cbCamera.Create(wnd, &cbCameraData, sizeof(cbCameraData));
	cbCamera.BindGS(1);

	// update constant buffer
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view = cam.GetMatrix();
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);

	DirectX::XMStoreFloat4x4(&cbDataWVP.matVP, XMMatrixTranspose(view * proj));
	DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(world));
	cbWVP.Update(&cbDataWVP);

	// blend state for rendering billboards
	ml::BlendState alphaBS;
	alphaBS.Info.AlphaToCoverageEnable = true;
	alphaBS.Create(wnd);
	alphaBS.Bind();

	// rasterizer state - turn off culling
	ml::RasterizerState rasterizer;
	rasterizer.Info.CullMode = D3D11_CULL_NONE;
	rasterizer.Create(wnd);
	rasterizer.Bind();

	// load billboard texture
	ml::Image img;
	ml::Texture tex;
	ml::ShaderResourceView srv;

	img.LoadFromFile("billboard.png");
	tex.Create(wnd, img);
	srv.Create(wnd, tex);
	srv.BindPS();

	// create sampler
	ml::SamplerState smp;
	smp.Create(wnd);
	smp.BindPS();

	ml::Event e;
	while (wnd.IsOpen()) {
		while (wnd.GetEvent(e)) {
			if (e.Type == ml::EventType::Scroll) {
				// tell the camera to zoom out/in
				cam.Move(-e.MouseWheel.Delta);

				// get the updated matrix
				view = cam.GetMatrix();

				// update constant buffer
				DirectX::XMStoreFloat4x4(&cbDataWVP.matVP, DirectX::XMMatrixTranspose(view * proj));
				cbWVP.Update(&cbDataWVP);
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

					// update constant buffer
					DirectX::XMStoreFloat4x4(&cbDataWVP.matVP, DirectX::XMMatrixTranspose(view * proj));
					cbWVP.Update(&cbDataWVP);
				}
			} else if (e.Type == ml::EventType::WindowResize) {
				proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);

				// update constant buffer
				DirectX::XMStoreFloat4x4(&cbDataWVP.matVP, DirectX::XMMatrixTranspose(view * proj));
				cbWVP.Update(&cbDataWVP);
			} else if (e.Type == ml::EventType::MouseButtonRelease)
				mousePressed = false;
		}

		// update cbCamera
		DirectX::XMStoreFloat3(&cbCameraData.viewPos, cam.GetPosition());
		cbCamera.Update(&cbCameraData);

		// clear back buffer and depth and stencil buffer
		wnd.Clear();
		wnd.ClearDepthStencil(1.0f, 0);

		// set topology and input signature
		wnd.SetTopology(ml::Topology::TriangleList);
		wnd.SetInputLayout(vertexInfo);

		// set shaders
		pxShader.Bind();
		vertShader.Bind();

		// bind data to shaders
		vertBuffer.Bind();

		// we should apply the view-projection matrix for the ground
		cbDataWVP.ApplyVP = 0.0f;
		cbWVP.Update(&cbDataWVP);

		// render the ground
		wnd.Draw(obj.GetObjectVertexCount("ground"), obj.GetObjectVertexStart("ground"));

		// we should not apply the view-projection matrix for the points in the vertex shader
		cbDataWVP.ApplyVP = 1.0f;
		cbWVP.Update(&cbDataWVP);

		// draw the billboards
		wnd.SetTopology(ml::Topology::PointList);
		geoShader.Bind();
		wnd.Draw(obj.GetObjectVertexCount("points"), obj.GetObjectVertexStart("points"));
		wnd.RemoveGeometryShader();

		// render everything
		wnd.Render();
	}

	wnd.Destroy();

	return 0;
}

