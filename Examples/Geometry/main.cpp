#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/Texture.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/SamplerState.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Base/GeometryFactory.h>
#include <MoonLight/Base/ShaderResourceView.h>
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
	vertexInfo.Add("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 16);

	// load pixel shader
	ml::PixelShader pxShader;
	pxShader.LoadFromFile(wnd, "pixel.hlsl", "main");

	// load vertex shader + create input signature
	ml::VertexShader vertShader;
	vertShader.InputSignature = &vertexInfo;
	vertShader.LoadFromFile(wnd, "vertex.hlsl", "main");

	// load geometry
	ml::Geometry rect2D = ml::GeometryFactory::CreateRectangle(25, -25, 50, 50, wnd);
	ml::Geometry circle2D = ml::GeometryFactory::CreateCircle(100, -100, 25, 25, wnd);
	ml::Geometry triangle2D = ml::GeometryFactory::CreateTriangle(75, 75, 25, wnd);
	ml::Geometry sphere = ml::GeometryFactory::CreateSphere(3, wnd);
	ml::Geometry cube = ml::GeometryFactory::CreateCube(8, 3, 3, wnd);
	ml::Geometry plane = ml::GeometryFactory::CreatePlane(3, 8, wnd);

	// load texture
	ml::Image tileImg;
	tileImg.LoadFromFile("tile.png");

	ml::Texture tile;
	tile.Create(wnd, tileImg);

	ml::ShaderResourceView tileSRV;
	tileSRV.Create(wnd, tile);
	tileSRV.BindPS();

	// create sampler state
	ml::SamplerState smp;
	smp.Create(wnd);
	smp.BindPS();

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
	DirectX::XMMATRIX proj = DirectX::XMMatrixOrthographicLH(wnd.GetSize().x, wnd.GetSize().y, 0.1f, 1000.0f);

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
			else if (e.Type == ml::EventType::WindowResize) {
				wnd.GetSize();
			}
		}
		
		// do we still have the window? NOTE: this is only used in this example since we use the wnd.GetSize() which throws
		// an error if the window is closed. another possible solution is to handle the events after the rendering
		if (!wnd.IsOpen()) break;

		// clear back buffer and depth and stencil buffer
		wnd.Clear();
		wnd.ClearDepthStencil(1.0f, 0);

		// set topology and input signature
		wnd.SetInputLayout(vertexInfo);

		// set shaders
		pxShader.Bind();
		vertShader.Bind();

		// bind data to shaders
		cbWVP.BindVS(0);			// bind cbWVP on first(0) slot


		// draw 3D stuff
		wnd.SetTopology(ml::Topology::TriangleList);
		proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(70), wnd.GetAspectRatio(), 0.1f, 1000.0f);
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixTranspose(world * view * proj));
		cbWVP.Update(&cbDataWVP);

		sphere.Draw();
		cube.Draw();
		plane.Draw();


		// draw 2D stuff
		wnd.SetTopology(ml::Topology::TriangleStrip);
		proj = DirectX::XMMatrixOrthographicLH(wnd.GetSize().x, wnd.GetSize().y, 0.1f, 1000.0f);
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixTranspose(world * proj));
		cbWVP.Update(&cbDataWVP);

		rect2D.Draw();
		circle2D.Draw();
		triangle2D.Draw();


		// render everything
		wnd.Render();
	}

	wnd.Destroy();

	return 0;
}

