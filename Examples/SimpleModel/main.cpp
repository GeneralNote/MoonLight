#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Model/OBJModel.h>
#include "../Common/Camera.h"

__declspec(align(16))
struct ConstantBufferWVP
{
	DirectX::XMFLOAT4X4 matWVP;
	DirectX::XMFLOAT4X4 matWorld;
};

int main()
{
	// window configuration
	ml::Window::Config wndConfig;
	wndConfig.Fullscreen = false;
	wndConfig.Sample.Count = 1;
	wndConfig.Sample.Quality = 0;

	// create engine
	ml::Window wnd;
	wnd.Create(DirectX::XMINT2(800, 600), "Test project!", ml::Window::Style::Resizable, wndConfig);
	
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
	vertShader.LoadFromFile(wnd, "vertex.hlsl", "main", true);

	// load model
	ml::OBJModel obj;
	obj.LoadFromFile("uh60.obj");

	// create vertex buffer
	ml::VertexBuffer<ml::OBJModel::Vertex> vertBuffer;
	vertBuffer.Create(wnd, obj.GetVertexData(), obj.GetVertexCount(), ml::Resource::Immutable);

	// generate shader data
	ConstantBufferWVP cbDataWVP;
	DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixIdentity());

	// create constant buffer
	ml::ConstantBuffer<ConstantBufferWVP> cbWVP;
	cbWVP.Create(wnd, &cbDataWVP, sizeof(cbDataWVP));

	// camera stuff
	mv::Camera cam;
	bool mousePressed = false;
	DirectX::XMINT2 mouseContact(0, 0);

	// update constant buffer
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view = cam.GetMatrix();
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);

	DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, XMMatrixTranspose(world * view * proj));
	DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(world));
	cbWVP.Update(&cbDataWVP);

	ml::Event e;
	while (wnd.IsOpen()) {
		while (wnd.GetEvent(e)) {
			if (e.Type == ml::EventType::Scroll) {
				// tell the camera to zoom out/in
				cam.Move(-e.MouseWheel.Delta);

				// get the updated matrix
				view = cam.GetMatrix();

				// update constant buffer
				DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixTranspose(world * view * proj));
				cbWVP.Update(&cbDataWVP);
			}
			else if (e.Type == ml::EventType::MouseButtonPress) {
				mousePressed = true;
				mouseContact = e.MouseButton.Position;
			}
			else if (e.Type == ml::EventType::MouseMove) {
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
					DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixTranspose(world * view * proj));
					cbWVP.Update(&cbDataWVP);
				}
			}
			else if (e.Type == ml::EventType::MouseButtonRelease)
				mousePressed = false;
			else if (e.Type == ml::EventType::WindowResize) {
				proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);
				DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixTranspose(world * view * proj));
				cbWVP.Update(&cbDataWVP);
			}
		}

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
		cbWVP.BindVS(0);			// bind cbWVP on first(0) slot
		vertBuffer.Bind();

		wnd.Draw(obj.GetVertexCount());

		// render everything
		wnd.Render();
	}

	wnd.Destroy();

	return 0;
}

