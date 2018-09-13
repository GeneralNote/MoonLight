#include <MoonLight/Base/HullShader.h>
#include <d3dcompiler.h>

namespace ml
{
	bool HullShader::LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile, const Shader::MacroList& macros, const IncludeHandler& include)
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
			HRESULT hr = D3DCompile(code, codeLen, "HullShader", macros.GetData(), &hInclude, entry.c_str(), "hs_5_0", flags, 0, outCode.GetAddressOf(), outMsgs.GetAddressOf());
			if (FAILED(hr)) {
				printf("[D3D] Failed to compile the hull shader");
				if (outMsgs != nullptr)
					printf(" (%s)", (char*)outMsgs->GetBufferPointer());
				printf(".\n");
				return false;
			}

			// create shader object with bytecode
			hr = wnd.GetDevice()->CreateHullShader(outCode->GetBufferPointer(), outCode->GetBufferSize(), nullptr, (ID3D11HullShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create hull shader.\n");
				return false;
			}
		} else {
			// create shader object
			HRESULT hr = wnd.GetDevice()->CreateHullShader(code, codeLen, nullptr, (ID3D11HullShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create hull shader.\n");
				return false;
			}
		}

		return true;
	}
	void HullShader::Bind()
	{
		assert(mWindow != nullptr);

		mWindow->GetDeviceContext()->HSSetShader((ID3D11HullShader*)mShader, nullptr, 0);
	}
}