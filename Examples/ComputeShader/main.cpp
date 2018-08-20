#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Event.h>
#include <MoonLight/Base/Buffer.h>
#include <MoonLight/Base/ComputeShader.h>
#include <MoonLight/Base/GenericBuffer.h>
#include <MoonLight/Base/ShaderResourceView.h>
#include <MoonLight/Base/UnorderedAccessView.h>
#include <time.h>

#define VECTOR_COUNT 64

struct Vector3D
{
	float X,Y,Z;
};

int main()
{
	// create engine
	ml::Window wnd;
	wnd.Create(DirectX::XMINT2(800, 600), "Particles", ml::Window::Style::Resizable);

	// load compute shader
	ml::ComputeShader computeShader;
	computeShader.LoadFromFile(wnd, "compute.hlsl", "main");

	// create a list of vectors with length between 1-10
	std::vector<Vector3D> vectors(VECTOR_COUNT);
	for (int i = 0; i < VECTOR_COUNT; i++)
		vectors[i] = { 1.0f + rand() % 10, 0.0f, 0.0f }; // super "random" vectors

	// create structured buffer
	ml::GenericBuffer<Vector3D> bVectors;
	bVectors.Create(wnd, vectors.data(), vectors.size(), ml::Resource::StructuredBuffer | ml::Resource::ShaderResource);

	// create shader resource view
	ml::ShaderResourceView vectorsView;
	vectorsView.Create(wnd, bVectors);

	// create "generic" buffer with float3 values
	ml::GenericBuffer<float> bLengths;
	bLengths.Create(wnd, vectors.size(), ml::Resource::UnorderedAccess | ml::Resource::CPURead);

	// create unordered access view
	ml::UnorderedAccessView lengthsView;
	lengthsView.Create(wnd, bLengths, DXGI_FORMAT_R32_FLOAT);

	// bind the resources
	computeShader.Bind();
	vectorsView.BindCS();
	lengthsView.Bind();

	// call the compute shader
	wnd.Compute(VECTOR_COUNT / 64, 1, 1);

	// read the lengths
	D3D11_MAPPED_SUBRESOURCE lengthData;
	wnd.GetDeviceContext()->Map(bLengths.GetBuffer(), 0, D3D11_MAP_READ, 0, &lengthData);
	float* dataView = reinterpret_cast<float*>(lengthData.pData);
	for (int i = 0; i < VECTOR_COUNT; ++i)
		printf("%.2f\n", dataView[i]);
	wnd.GetDeviceContext()->Unmap(bLengths.GetBuffer(), 0);

	// remove the compute shader
	wnd.RemoveComputeShader();

	ml::Event e;
	while (wnd.IsOpen()) {
		while (wnd.GetEvent(e)) { }

		// clear back buffer and depth-stencil buffer
		wnd.Clear();
		wnd.ClearDepthStencil(1.0f, 0);

		// render everything
		wnd.Render();
	}

	wnd.Destroy();

	return 0;
}

