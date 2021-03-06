#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/HullShader.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/DomainShader.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Base/RasterizerState.h>
#include <MoonLight/Model/OBJModel.h>
#include "../Common/Camera.h"

__declspec(align(16))
struct ConstantBufferWVP
{
	DirectX::XMFLOAT4X4 matWVP;
	DirectX::XMFLOAT3 CameraPosition;
};

int main()
{
	// create engine
	ml::Window wnd;
	wnd.Create(DirectX::XMINT2(800, 600), "Test project!", ml::Window::Style::Default);

	// describe our vertex data
	ml::VertexInputLayout vertexInfo;
	vertexInfo.Add("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0);

	// load pixel shader
	ml::PixelShader pxShader;
	pxShader.LoadFromFile(wnd, "shaders.hlsl", "mainPS");

	// load vertex shader + create input signature
	ml::VertexShader vertShader;
	vertShader.InputSignature = &vertexInfo;
	vertShader.LoadFromFile(wnd, "shaders.hlsl", "mainVS");

	// load hull and domain shaders
	ml::HullShader hullShader;
	hullShader.LoadFromFile(wnd, "shaders.hlsl", "mainHS");

	// load domain shader
	ml::DomainShader domainShader;
	domainShader.LoadFromFile(wnd, "shaders.hlsl", "mainDS");

	// load model
	ml::OBJModel obj;
	obj.LoadFromFile("quad.obj");

	// create vertex buffer
	ml::VertexBuffer<ml::OBJModel::Vertex> vertBuffer;
	vertBuffer.Create(wnd, obj.GetVertexData(), obj.GetVertexCount(), ml::Resource::Immutable);

	// camera stuff
	mv::Camera cam;
	bool mousePressed = false;
	DirectX::XMINT2 mouseContact(0, 0);

	// update constant buffer
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view = cam.GetMatrix();
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);

	// generate shader data
	ConstantBufferWVP cbData;
	DirectX::XMStoreFloat4x4(&cbData.matWVP, DirectX::XMMatrixTranspose(world*view*proj));
	DirectX::XMStoreFloat3(&cbData.CameraPosition, cam.GetPosition());

	// create constant buffer
	ml::ConstantBuffer<ConstantBufferWVP> cb;
	cb.Create(wnd, &cbData, sizeof(cbData));

	// default rasterizer state (wireframe)
	ml::RasterizerState raster;
	raster.Info.CullMode = D3D11_CULL_NONE;
	raster.Info.FillMode = D3D11_FILL_WIREFRAME;
	raster.Create(wnd);
	raster.Bind();

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
			} else if (e.Type == ml::EventType::MouseButtonRelease)
				mousePressed = false;
		}

		DirectX::XMStoreFloat4x4(&cbData.matWVP, DirectX::XMMatrixTranspose(world*view*proj));
		DirectX::XMStoreFloat3(&cbData.CameraPosition, cam.GetPosition());
		cb.Update(&cbData);

		// clear back buffer and depth and stencil buffer
		wnd.Clear();
		wnd.ClearDepthStencil(1.0f, 0);

		// set topology and input signature
		wnd.SetTopology(ml::Topology::ControlPointList(4));
		wnd.SetInputLayout(vertexInfo);

		// set shaders
		pxShader.Bind();
		vertShader.Bind();
		hullShader.Bind();
		domainShader.Bind();

		// bind data to shaders
		cb.BindHS();
		cb.BindDS();
		vertBuffer.Bind();

		wnd.Draw(obj.GetVertexCount()-2);

		// render everything
		wnd.Render();
	}

	wnd.Destroy();

	return 0;
}

