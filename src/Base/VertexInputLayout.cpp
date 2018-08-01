#include <MoonLight/Base/VertexInputLayout.h>
#include <MoonLight/Base/Window.h>

namespace ml
{
	VertexInputLayout::VertexInputLayout()
	{
		mLayout = nullptr;
	}
	VertexInputLayout::~VertexInputLayout()
	{
		for (int i = 0; i < mElements.size(); ++i)
			free((char*)mElements[i].SemanticName);
	}
	void VertexInputLayout::Add(const std::string& semantic, ml::UInt32 semanticIndex, DXGI_FORMAT fmt, ml::UInt32 offset, ml::UInt32 inputSlot, D3D11_INPUT_CLASSIFICATION classification, ml::UInt32 stepRate)
	{
		char *semanticLPCSTR = (char*)malloc(semantic.size() + 1);
		memcpy(semanticLPCSTR, semantic.c_str(), semantic.size());
		semanticLPCSTR[semantic.size()] = 0;

		mElements.push_back({ semanticLPCSTR, semanticIndex, fmt, inputSlot, offset, classification, stepRate });
	}
	void VertexInputLayout::Create(ml::Window& wnd, const char* vShaderCode, ml::UInt32 vShaderLength)
	{
		HRESULT hr = wnd.GetDevice()->CreateInputLayout(mElements.data(), mElements.size(), vShaderCode, vShaderLength, mLayout.GetAddressOf());
		if (FAILED(hr))
			printf("[D3D] Failed to create ID3D11InputLayout.\n");
	}
}