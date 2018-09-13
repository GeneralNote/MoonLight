#include <MoonLight/Base/ComputeShader.h>
#include <d3dcompiler.h>

namespace ml
{
	bool ComputeShader::LoadFromMemory(ml::Window & wnd, const char * code, ml::UInt32 codeLen, std::string entry, bool needsCompile, const Shader::MacroList& macros)
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

			// compile
			HRESULT hr = D3DCompile(code, codeLen, "ComputeShader", macros.GetData(), nullptr, entry.c_str(), "cs_5_0", flags, 0, outCode.GetAddressOf(), outMsgs.GetAddressOf());
			if (FAILED(hr)) {
				printf("[D3D] Failed to compile the compute shader");
				if (outMsgs != nullptr)
					printf(" (%s)", (char*)outMsgs->GetBufferPointer());
				printf(".\n");
				return false;
			}

			// create shader object with bytecode
			hr = wnd.GetDevice()->CreateComputeShader(outCode->GetBufferPointer(), outCode->GetBufferSize(), nullptr, (ID3D11ComputeShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create compute shader.\n");
				return false;
			}
		} else {
			// create shader object
			HRESULT hr = wnd.GetDevice()->CreateComputeShader(code, codeLen, nullptr, (ID3D11ComputeShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create compute shader.\n");
				return false;
			}
		}

		return true;
	}
	void ComputeShader::Bind()
	{
		mWindow->GetDeviceContext()->CSSetShader((ID3D11ComputeShader*)mShader, nullptr, 0);
	}
}