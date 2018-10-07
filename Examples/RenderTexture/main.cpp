#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/SamplerState.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/RenderTexture.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Base/GeometryFactory.h>
#include <MoonLight/Base/ShaderResourceView.h>
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
	vertShader.LoadFromFile(wnd, "vertex.hlsl", "main");


	// describe our vertex data
	ml::VertexInputLayout vertexInfo2D;
	vertexInfo2D.Add("POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0);
	vertexInfo2D.Add("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 16);

	// load pixel shader
	ml::PixelShader pxShader2D;
	pxShader2D.LoadFromFile(wnd, "pixel2D.hlsl", "main");

	// load vertex shader + create input signature
	ml::VertexShader vertShader2D;
	vertShader2D.InputSignature = &vertexInfo2D;
	vertShader2D.LoadFromFile(wnd, "vertex2D.hlsl", "main");


	// create geometry for "minimap"
	ml::Geometry minimap = ml::GeometryFactory::CreateRectangle(10, -10, 275, 275*(3/4.f), wnd);

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
	cbWVP.BindVS(0);

	// camera stuff
	mv::Camera cam;
	bool mousePressed = false;
	DirectX::XMINT2 mouseContact(0, 0);

	// create render texture
	ml::RenderTexture rt;
	rt.Create(wnd, DirectX::XMINT2(800, 600), ml::Resource::ShaderResource, true);

	// create shader resource view to render texture
	ml::ShaderResourceView rtSRV;
	rtSRV.Create(wnd, rt);

	// create sampler
	ml::SamplerState sampler;
	sampler.Create(wnd);

	// update constant buffer
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view = cam.GetMatrix();
	DirectX::XMMATRIX viewTopDown = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(180)) * DirectX::XMMatrixTranslation(0, 2.5f, 30);
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);
	DirectX::XMMATRIX proj2D = DirectX::XMMatrixOrthographicOffCenterLH(0, wnd.GetSize().x, -wnd.GetSize().y, 0, 0.1f, 1000.0f);

	DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, XMMatrixTranspose(world * view * proj));
	DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(world));
	cbWVP.Update(&cbDataWVP);

	// we want to render to the rendertexture
	rt.Bind();
	rt.Clear();
	rt.ClearDepthStencil(1.0f, 0);

	// update cb
	DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixTranspose(world * viewTopDown * proj));
	cbWVP.Update(&cbDataWVP);

	// render the 3D model
	wnd.SetTopology(ml::Topology::TriangleList);
	wnd.SetInputLayout(vertexInfo);
	pxShader.Bind();
	vertShader.Bind();
	vertBuffer.Bind();

	wnd.Draw(obj.GetVertexCount());

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
				proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);
				proj2D = DirectX::XMMatrixOrthographicOffCenterLH(0, wnd.GetSize().x, -wnd.GetSize().y, 0, 0.1f, 1000.0f);
			}
		}

		// we want to render to the window now
		wnd.Bind();

		// clear back buffer and depth and stencil buffer for the main rendering part
		wnd.Clear();
		wnd.ClearDepthStencil(1.0f, 0);

		// update constant buffer
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixTranspose(world * proj2D));
		cbWVP.Update(&cbDataWVP);

		// render the minimap
		wnd.SetTopology(ml::Topology::TriangleStrip);
		wnd.SetInputLayout(vertexInfo2D);
		rtSRV.BindPS();
		sampler.BindPS();
		pxShader2D.Bind();
		vertShader2D.Bind();

		minimap.Draw();


		// update cb
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixTranspose(world * view * proj));
		cbWVP.Update(&cbDataWVP);

		// render the 3D model
		wnd.SetTopology(ml::Topology::TriangleList);
		wnd.SetInputLayout(vertexInfo);
		pxShader.Bind();
		vertShader.Bind();
		vertBuffer.Bind();

		wnd.Draw(obj.GetVertexCount());


		// render everything
		wnd.Render();
	}

	wnd.DebugDumpDirect3DLiveObjects();
	wnd.Destroy();
	
	return 0;
}

