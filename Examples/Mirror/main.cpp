#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/PixelShader.h>
#include <MoonLight/Base/VertexShader.h>
#include <MoonLight/Base/VertexBuffer.h>
#include <MoonLight/Base/ConstantBuffer.h>
#include <MoonLight/Base/RasterizerState.h>
#include <MoonLight/Base/DepthStencilState.h>
#include <MoonLight/Base/BlendState.h>
#include <MoonLight/Model/OBJModel.h>
#include "../Common/Camera.h"

__declspec(align(16))
struct ConstantBufferWVP
{
	DirectX::XMFLOAT4X4 matVP;
	DirectX::XMFLOAT4X4 matWorld;
	float Transparency;
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
	obj.LoadFromFile("scene.obj");

	// create vertex buffer
	ml::VertexBuffer<ml::OBJModel::Vertex> vertBuffer;
	vertBuffer.Create(wnd, obj.GetVertexData(), obj.GetVertexCount(), ml::Resource::Immutable);

	// generate shader data
	ConstantBufferWVP cbDataWVP;

	// create constant buffer
	ml::ConstantBuffer<ConstantBufferWVP> cbWVP;
	cbWVP.Create(wnd, &cbDataWVP, sizeof(cbDataWVP));

	// camera stuff
	mv::Camera cam;
	bool mousePressed = false;
	DirectX::XMINT2 mouseContact(0, 0);

	// update constant buffer
	DirectX::XMMATRIX viewAddRota = DirectX::XMMatrixRotationY(DirectX::XM_PI / 1.1f);
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX view = viewAddRota*cam.GetMatrix();
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);

	DirectX::XMStoreFloat4x4(&cbDataWVP.matVP, XMMatrixTranspose(view * proj));
	DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(world));
	cbWVP.Update(&cbDataWVP);

	// turn off culling for all objects
	ml::RasterizerState rasterizer;
	rasterizer.Info.CullMode = D3D11_CULL_NONE;
	rasterizer.Create(wnd);
	rasterizer.Bind();
	
	// blend state for not rendering onto back buffer
	ml::BlendState disableWriteBS;
	disableWriteBS.Info.RenderTarget[0].RenderTargetWriteMask = 0;
	disableWriteBS.Create(wnd);

	// blend state for rendering transparent objects
	ml::BlendState alphaBS;
	alphaBS.Create(wnd);

	// depth stencil state for rendering only onto stencil buffer
	ml::DepthStencilState renderMirrorDSS;
	renderMirrorDSS.Info.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	renderMirrorDSS.Info.StencilEnable = true;
	renderMirrorDSS.Info.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	renderMirrorDSS.Info.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	renderMirrorDSS.Info.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	renderMirrorDSS.Info.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	renderMirrorDSS.Create(wnd);

	// depth stencil state for rendering reflection
	ml::DepthStencilState renderReflectDSS;
	renderReflectDSS.Info.StencilEnable = true;
	renderReflectDSS.Info.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	renderReflectDSS.Info.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	renderReflectDSS.Info.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	renderReflectDSS.Info.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	renderReflectDSS.Create(wnd);

	// rasterizer state for reflected triangles
	ml::RasterizerState reflectedRS;
	reflectedRS.Info.CullMode = D3D11_CULL_NONE;
	reflectedRS.Info.FrontCounterClockwise = true;
	reflectedRS.Create(wnd);

	// calculate reflection plane
	ml::Bounds3D mirrorBounds = obj.GetObjectBounds("Mirror");
	ml::OBJModel::Vertex* mirrorVerts = obj.GetObjectVertices("Mirror");

	DirectX::XMVECTOR mirrorP1 = DirectX::XMVectorSet(mirrorVerts[0].Position.x, mirrorVerts[0].Position.y, mirrorVerts[0].Position.z, 0);
	DirectX::XMVECTOR mirrorP2 = DirectX::XMVectorSet(mirrorVerts[1].Position.x, mirrorVerts[1].Position.y, mirrorVerts[1].Position.z, 0);
	DirectX::XMVECTOR mirrorP3 = DirectX::XMVectorSet(mirrorVerts[2].Position.x, mirrorVerts[2].Position.y, mirrorVerts[2].Position.z, 0);
	DirectX::XMVECTOR mirrorPlane = DirectX::XMVectorNegate(DirectX::XMPlaneFromPoints(mirrorP1, mirrorP2, mirrorP3));

	ml::Event e;
	while (wnd.IsOpen()) {
		while (wnd.GetEvent(e)) {
			if (e.Type == ml::EventType::Scroll) {
				// tell the camera to zoom out/in
				cam.Move(-e.MouseWheel.Delta);

				// get the updated matrix
				view = viewAddRota *cam.GetMatrix();

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
					view = viewAddRota *cam.GetMatrix();

					// update constant buffer
					DirectX::XMStoreFloat4x4(&cbDataWVP.matVP, DirectX::XMMatrixTranspose(view * proj));
					cbWVP.Update(&cbDataWVP);
				}
			}
			else if (e.Type == ml::EventType::WindowResize) {
				proj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45), wnd.GetAspectRatio(), 0.1f, 1000.0f);
				
				// update constant buffer
				DirectX::XMStoreFloat4x4(&cbDataWVP.matVP, DirectX::XMMatrixTranspose(view * proj));
				cbWVP.Update(&cbDataWVP);
			}
			else if (e.Type == ml::EventType::MouseButtonRelease)
				mousePressed = false;
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
		cbWVP.BindVS();			// bind cbWVP on first(0) slot
		vertBuffer.Bind();

		// render everything with no transform
		world = DirectX::XMMatrixIdentity();
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(world));
		cbDataWVP.Transparency = 1.0f;
		cbWVP.Update(&cbDataWVP);

		// render static mesh normally
		wnd.Draw(obj.GetObjectVertexCount("Static"), obj.GetObjectVertexStart("Static"));

		// render mirror to the stencil buffer
		disableWriteBS.Bind();
		renderMirrorDSS.Bind(1);
		wnd.Draw(obj.GetObjectVertexCount("Mirror"), obj.GetObjectVertexStart("Mirror"));
		wnd.RemoveBlendState();

		// calculate reflection matrix
		world = DirectX::XMMatrixReflect(mirrorPlane);
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(world));
		cbWVP.Update(&cbDataWVP);

		// render data that is in the mirror
		reflectedRS.Bind();
		renderReflectDSS.Bind(1);
		wnd.Draw(obj.GetObjectVertexCount("Static"), obj.GetObjectVertexStart("Static"));
		wnd.RemoveDepthStencilState();

		// render the mirror in its place now
		world = DirectX::XMMatrixIdentity();
		DirectX::XMStoreFloat4x4(&cbDataWVP.matWorld, XMMatrixTranspose(world));
		cbDataWVP.Transparency = 0.5f;
		cbWVP.Update(&cbDataWVP);

		// render the mirror with blending
		rasterizer.Bind();
		alphaBS.Bind();
		wnd.Draw(obj.GetObjectVertexCount("Mirror"), obj.GetObjectVertexStart("Mirror"));
		wnd.RemoveBlendState();


		// render everything
		wnd.Render();
	}

	wnd.Destroy();

	return 0;
}

