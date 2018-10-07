#ifndef __MOONLIGHT_INPUT_LAYOUT_H__
#define __MOONLIGHT_INPUT_LAYOUT_H__

#include <MoonLight/Base/Math.h>
#include <MoonLight/Base/Ptr.h>
#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include <string>

namespace ml
{
	class Window;

	class VertexInputLayout
	{
	public:
		VertexInputLayout();
		~VertexInputLayout();

		void Add(const std::string& semantic, ml::UInt32 semanticIndex, DXGI_FORMAT fmt, ml::UInt32 offset, ml::UInt32 inputSlot = 0, D3D11_INPUT_CLASSIFICATION classification = D3D11_INPUT_PER_VERTEX_DATA, ml::UInt32 stepRate = 0);
		void Create(ml::Window& wnd, const char* vShaderCode, ml::UInt32 vShaderLength);

		inline ID3D11InputLayout* GetInputLayout() { return mLayout.Get(); }
		inline std::vector<D3D11_INPUT_ELEMENT_DESC>& GetInputElements() { return mElements; }

	private:
		ml::Ptr<ID3D11InputLayout> mLayout;
		std::vector<D3D11_INPUT_ELEMENT_DESC> mElements;
	};
}

#endif // __MOONLIGHT_INPUT_SIGNATURE_H__