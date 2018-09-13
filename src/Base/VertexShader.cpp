#include <MoonLight/Base/VertexShader.h>
#include <d3dcompiler.h>

namespace ml
{
	VertexShader::VertexShader() :
		Shader()
	{
		InputSignature = nullptr;
	}
	
	bool VertexShader::LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile, const Shader::MacroList& macros, const IncludeHandler& include)
	{
		mWindow = &wnd;

		if (mShader != nullptr) {
			mShader->Release();
			mShader = nullptr;
		}

		if (needsCompile) {
			// compilation flags
			UINT flags = 0;
#ifdef _DEBUG
			flags |= D3DCOMPILE_DEBUG;
#endif

			// blobs for holding compile output data
			ml::Ptr<ID3DBlob> outCode, outMsgs;

			// create actual include handler
			priv_impl::ID3DIncludeHandler hInclude(const_cast<IncludeHandler*>(&include));

			// compile
			HRESULT hr = D3DCompile(code, codeLen, "VertexShader", macros.GetData(), &hInclude, entry.c_str(), "vs_5_0", flags, 0, outCode.GetAddressOf(), outMsgs.GetAddressOf());
			if (FAILED(hr)) {
				printf("[D3D] Failed to compile the vertex shader");
				if (outMsgs != nullptr)
					printf(" (%s)", (char*)outMsgs->GetBufferPointer());
				printf(".\n");
				return false;
			}

			// create shader object with bytecode
			hr = wnd.GetDevice()->CreateVertexShader(outCode->GetBufferPointer(), outCode->GetBufferSize(), nullptr, (ID3D11VertexShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create vertex shader.\n");
				return false;
			}

			// create input signature if its bound to this vertex shader
			mCreateInputSignature(wnd, (char*)outCode->GetBufferPointer(), outCode->GetBufferSize());
		} else {
			// create shader object
			HRESULT hr = wnd.GetDevice()->CreateVertexShader(code, codeLen, nullptr, (ID3D11VertexShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create vertex shader.\n");
				return false;
			}

			// create input signature if its bound to this vertex shader
			mCreateInputSignature(wnd, code, codeLen);
		}

		return true;
	}
	void VertexShader::Bind()
	{
		assert(mWindow != nullptr);

		mWindow->GetDeviceContext()->VSSetShader((ID3D11VertexShader*)mShader, nullptr, 0);
	}
	void VertexShader::mCreateInputSignature(ml::Window& wnd, const char* bc, ml::UInt32 bcLen)
	{
		if (InputSignature != nullptr)
			if (InputSignature->GetInputLayout() == nullptr) // create the input signature only if needed
				InputSignature->Create(wnd, bc, bcLen);
	}
}