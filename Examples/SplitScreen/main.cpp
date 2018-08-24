#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Base/Viewport.h>


struct MyVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};

__declspec(align(16))
struct ConstantBufferWVP
{
	DirectX::XMFLOAT4X4 matWVP;
	int Viewport;
};

int main()
{
	// create engine
	ml::Window wnd;
	wnd.Create(DirectX::XMINT2(800, 600), "Test project!", ml::Window::Style::Default);

	// simple engine and window property changes
	wnd.SetClearColor(ml::Color(0, 255, 0));

	// describe our vertex data
	ml::VertexInputLayout vertexInfo;
	vertexInfo.Add("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0);
	vertexInfo.Add("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 12);

	// load pixel shader
	ml::PixelShader pxShader;
	pxShader.LoadFromFile(wnd, "pixel.hlsl", "main");

	// load vertex shader + create input signature
	ml::VertexShader vertShader;
	vertShader.InputSignature = &vertexInfo;
	vertShader.LoadFromFile(wnd, "vertex.hlsl", "main");

	// generate triangle
	std::vector<MyVertex> verts(3);
	verts[0] = { DirectX::XMFLOAT3(0.5f, 0.f, 0.f), DirectX::XMFLOAT2(1.0f, 1.0f) };
	verts[1] = { DirectX::XMFLOAT3(-0.5f, 0.0f, 0.f), DirectX::XMFLOAT2(0.0f, 1.0f) };
	verts[2] = { DirectX::XMFLOAT3(0.0f, 0.5f, 0.0f), DirectX::XMFLOAT2(0.5f, 0.0f) };

	// create vertex buffer
	ml::VertexBuffer<MyVertex> vertBuffer;
	vertBuffer.Create(wnd, verts.data(), verts.size(), ml::Resource::Immutable);

	// generate shader data
	ConstantBufferWVP cbDataWVP;
	DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixIdentity());

	// create constant buffer
	ml::ConstantBuffer<ConstantBufferWVP> cbWVP;
	cbWVP.Create(wnd, &cbDataWVP, sizeof(cbDataWVP));

	// create viewports
	ml::Viewport views;
	views.X[0] = 0;
	views.Y[0] = 0;
	views.Width[0] = wnd.GetSize().x / 2;
	views.Height[0] = wnd.GetSize().y;
	views.X[1] = views.Width[0];
	views.Y[1] = 0;
	views.Width[1] = wnd.GetSize().x / 2;
	views.Height[1] = wnd.GetSize().y;
	views.Bind(wnd, 2);

	ml::Event e;
	while (wnd.IsOpen()) {
		while (wnd.GetEvent(e)) {
			if (e.Type == ml::EventType::WindowResize)
				views.Bind(wnd, 2);
		}

		DirectX::XMMATRIX wvp = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(45)) * DirectX::XMMatrixTranslation(0, 0, 7);
		wvp = wvp * DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), (float)wnd.GetSize().x/2/wnd.GetSize().y, 0.1f, 1000.0f);
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, XMMatrixTranspose(wvp));
		
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
		cbWVP.BindVS();
		vertBuffer.Bind();

		/* RENDER TO THE FIRST VIEW */

		// update constant buffer
		cbDataWVP.Viewport = 0;
		cbWVP.Update(&cbDataWVP);

		// render vertex buffer
		wnd.Draw(verts.size());

		/* RENDER TO THE SECOND VIEW */

		// update constant buffer
		cbDataWVP.Viewport = 1;
		cbWVP.Update(&cbDataWVP);

		// render vertex buffer
		wnd.Draw(verts.size());

		// render everything
		wnd.Render();
	}

	return 0;
}

