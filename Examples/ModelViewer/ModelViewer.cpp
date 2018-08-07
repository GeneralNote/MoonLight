#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Base/Texture.h>
#include <MoonLight/Base/Image.h>
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
	vertexInfo.Add("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 24);

	// load pixel shader
	ml::PixelShader pxShader;
	pxShader.LoadFromFile(wnd, "pixel.hlsl", "main");

	// load vertex shader + create input signature
	ml::VertexShader vertShader;
	vertShader.InputSignature = &vertexInfo;
	vertShader.LoadFromFile(wnd, "vertex.hlsl", "main");

	// load model
	ml::OBJModel obj;
	obj.LoadFromFile("car.obj");

	// load all images, create textures and views
	struct OBJTexBind
	{
		ml::Texture Texture;
		ml::Image Image;
		ml::ShaderResourceView View;
	};
	std::unordered_map<std::string, OBJTexBind> objTexData;
	std::vector<ml::OBJModel::MaterialData> matData = obj.GetMaterialList();
	for (ml::OBJModel::MaterialData mat : matData) {
		if (mat.Texture.empty() || objTexData.count(mat.Texture) > 0)
			continue;
		objTexData[mat.Texture].Image.LoadFromFile(mat.Texture);
		objTexData[mat.Texture].Texture.Create(wnd, objTexData[mat.Texture].Image);
		objTexData[mat.Texture].View.Create(wnd, objTexData[mat.Texture].Texture);
	}

	// create sampler state
	ml::SamplerState sampler;
	sampler.Create(wnd);

	// get list of the groups
	std::vector<std::string> objGroups = obj.GetObjects();

	// create vertex buffer
	ml::VertexBuffer<ml::OBJModel::Vertex> vertBuffer;
	vertBuffer.Create(wnd, obj.GetVertexData(), obj.GetVertexCount(), ml::Resource::Immutable);

	// generate shader data
	ConstantBufferWVP cbDataWVP;
	DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, DirectX::XMMatrixIdentity());

	// create constant buffer
	ml::ConstantBuffer<ConstantBufferWVP> cbWVP;
	cbWVP.Create(wnd, &cbDataWVP, sizeof(cbDataWVP), ml::Resource::Default);

	// create material constant buffer
	ml::ConstantBuffer<ml::OBJModel::Material> cbMaterial;
	cbMaterial.Create(wnd, sizeof(ml::OBJModel::Material));

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
				}
			}
			else if (e.Type == ml::EventType::MouseButtonRelease)
				mousePressed = false;
		}

		// update WVP const buffer once per frame
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, XMMatrixTranspose(DirectX::XMMatrixIdentity() * view * proj));
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(DirectX::XMMatrixIdentity()));
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
		cbWVP.BindVS(0);			// bind cbWVP on first(0) slot
		cbMaterial.BindVS(1);		// bind cbMaterial on second(1) slot
		vertBuffer.Bind();
		sampler.BindPS();

		for (const std::string& gname : objGroups) {
			std::pair<ml::OBJModel::MaterialData*, ml::UInt32>* mats = obj.GetObjectMaterials(gname);
			ml::UInt32 matCount = obj.GetObjectMaterialCount(gname);

			for (ml::UInt32 i = 0; i < matCount; i++) {
				// get the vertex start for this groups material
				ml::UInt32 matVStart = mats[i].second;

				// calculate the number of vertices to draw
				ml::UInt32 matVCount = 0;
				if (i != matCount - 1)
					matVCount = mats[i + 1].second - matVStart;
				else
					matVCount = obj.GetObjectVertexCount(gname) + obj.GetObjectVertexStart(gname) - matVStart;

				// update the object material
				cbMaterial.Update(&mats[i].first->Material);

				// bind the right texture
				if (!mats[i].first->Texture.empty())
					objTexData[mats[i].first->Texture].View.BindPS();

				// draw the group/object part with this material
				wnd.Draw(matVCount, matVStart);
			}
		}

		// render everything
		wnd.Render();
	}

	wnd.Destroy();

	return 0;
}

