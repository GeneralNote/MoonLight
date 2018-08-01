#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Model/OBJModel.h>
#include "Camera.h"

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

	// load pixel shader
	ml::PixelShader pxShader;
	pxShader.LoadFromFile(wnd, "pixel.hlsl", "main");

	// load vertex shader + create input signature
	ml::VertexShader vertShader;
	vertShader.InputSignature = &vertexInfo;
	vertShader.LoadFromFile(wnd, "vertex.hlsl", "main");

	// load model
	ml::OBJModel obj;
	obj.LoadFromFile("uh60.obj");

	// get rear and main rotor center
	DirectX::XMFLOAT3 cRearRotor = obj.GetGroupBounds("rear_rotor").Center();
	DirectX::XMFLOAT3 cMainRotor = obj.GetGroupBounds("main_rotor").Center();

	// get list of the groups
	std::vector<std::string> objGroups = obj.GetGroups();

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

		// calculate rotation for the rear/main rotor
		// NOTE: its frame dependant! i was too lazy
		static float rota = 0;
		rota += 1.f;

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

		for (const std::string& gname : objGroups) {
			std::pair<ml::OBJModel::Material*, ml::UInt32>* mats = obj.GetGroupMaterials(gname);
			ml::UInt32 matCount = obj.GetGroupMaterialCount(gname);

			// rotate main_rotor and rear_rotor groups
			if (gname == "main_rotor") {
				// rotate the object
				world = DirectX::XMMatrixTranslation(-cMainRotor.x, -cMainRotor.y, -cMainRotor.z) *		// center in the world origin first
					DirectX::XMMatrixRotationZ(DirectX::XMConvertToRadians(rota)) *						// rotate around origin
					DirectX::XMMatrixTranslation(cMainRotor.x, cMainRotor.y, cMainRotor.z);				// restore its original position


				DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, XMMatrixTranspose(world * view * proj));
				DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(world));
				cbWVP.Update(&cbDataWVP);
			}
			else if (gname == "rear_rotor") {
				// rotate the object
				world = DirectX::XMMatrixTranslation(-cRearRotor.x, -cRearRotor.y, -cRearRotor.z) *		// center in the world origin first
					DirectX::XMMatrixRotationX(DirectX::XMConvertToRadians(rota)) *						// rotate around origin
					DirectX::XMMatrixTranslation(cRearRotor.x, cRearRotor.y, cRearRotor.z);				// restore its original position

				DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, XMMatrixTranspose(world * view * proj));
				DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(world));
				cbWVP.Update(&cbDataWVP);
			}
			else {
				DirectX::XMStoreFloat4x4(&cbDataWVP.matWVP, XMMatrixTranspose(DirectX::XMMatrixIdentity() * view * proj));
				DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(DirectX::XMMatrixIdentity()));
				cbWVP.Update(&cbDataWVP);
			}

			for (ml::UInt32 i = 0; i < matCount; i++) {
				// get the vertex start for this groups material
				ml::UInt32 matVStart = mats[i].second;

				// calculate the number of vertices to draw
				ml::UInt32 matVCount = 0;
				if (i != matCount - 1)
					matVCount = mats[i + 1].second - matVStart;
				else
					matVCount = obj.GetGroupVertexCount(gname) + obj.GetGroupVertexStart(gname) - matVStart;

				// update the material material
				cbMaterial.Update(mats[i].first);

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

