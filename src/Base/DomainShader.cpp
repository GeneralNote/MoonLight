#include <MoonLight/Base/DomainShader.h>
#include <d3dcompiler.h>

namespace ml
{
	bool DomainShader::LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile)
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
			HRESULT hr = D3DCompile(code, codeLen, "DomainShader", nullptr, nullptr, entry.c_str(), "ds_5_0", flags, 0, outCode.GetAddressOf(), outMsgs.GetAddressOf());
			if (FAILED(hr)) {
				printf("[D3D] Failed to compile the hull shader");
				if (outMsgs != nullptr)
					printf(" (%s)", (char*)outMsgs->GetBufferPointer());
				printf(".\n");
				return false;
			}

			// create shader object with bytecode
			hr = wnd.GetDevice()->CreateDomainShader(outCode->GetBufferPointer(), outCode->GetBufferSize(), nullptr, (ID3D11DomainShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create hull shader.\n");
				return false;
			}
		} else {
			// create shader object
			HRESULT hr = wnd.GetDevice()->CreateDomainShader(code, codeLen, nullptr, (ID3D11DomainShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create hull shader.\n");
				return false;
			}
		}

		return true;
	}
	void DomainShader::Bind()
	{
		mWindow->GetDeviceContext()->DSSetShader((ID3D11DomainShader*)mShader, nullptr, 0);
	}
}