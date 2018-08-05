#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/Timer.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/ConstantBuffer.h>


struct MyVertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
};

__declspec(align(16))
struct ConstantBufferWVP
{
	DirectX::XMFLOAT4X4 matWVP;
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
	cbWVP.Create(wnd, &cbDataWVP, sizeof(cbDataWVP), ml::Resource::Default);

	// timer
	ml::Timer timer;

	ml::Event e;
	while (wnd.IsOpen()) {
		while (wnd.GetEvent(e)) { 
			if (e.Type == ml::EventType::KeyPress) {

				if (e.Keyboard.VK == 'P') {
					if (!timer.IsPaused()) timer.Pause();
					else timer.Resume();
				}
			}
		}

		// update constant buffer - rotate the triangle 360 degrees in 1 second using timer
		DirectX::XMMATRIX wvp = DirectX::XMMatrixRotationY(DirectX::XMConvertToRadians(timer.GetElapsedTime() * 360)) * DirectX::XMMatrixTranslation(0, 0, 7);
		wvp = wvp * DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, XMMatrixTranspose(wvp));
		cbWVP.Update(&cbDataWVP);

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
		
		// render vertex buffer
		wnd.Draw(verts.size());

		// render everything
		wnd.Render();
	}

	return 0;
}

