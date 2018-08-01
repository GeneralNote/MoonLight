#include <MoonLight/Base/PixelShader.h>
#include <d3dcompiler.h>

namespace ml
{
	bool PixelShader::LoadFromFile(ml::Window& wnd, std::string filename, std::string entry, bool needsCompile)
	{
		mWindow = &wnd;

		if (mShader != nullptr) {
			mShader->Release();
			mShader = nullptr;
		}

		if (needsCompile) {
			// convert filename
			std::wstring wfilen(filename.begin(), filename.end());

			// compilation flags
			UINT flags = 0;
#ifdef _DEBUG
			flags |= D3DCOMPILE_DEBUG;
#endif

			// blobs for holding compile output data
			ml::Ptr<ID3DBlob> outCode, outMsgs;

			// compile
			HRESULT hr = D3DCompileFromFile(wfilen.c_str(), nullptr, nullptr, entry.c_str(), "ps_5_0", flags, 0, outCode.GetAddressOf(), outMsgs.GetAddressOf());
			if (FAILED(hr)) {
				printf("[D3D] Failed to compile the pixel shader");
				if (outMsgs != nullptr)
					printf(" (%s)", (char*)outMsgs->GetBufferPointer());
				printf(".\n");

				return false;
			}

			// create shader object with bytecode
			hr = wnd.GetDevice()->CreatePixelShader(outCode->GetBufferPointer(), outCode->GetBufferSize(), nullptr, (ID3D11PixelShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create pixel shader.\n");
				return false;
			}
		} else {
			// open file
			FILE *file = fopen(filename.c_str(), "rb");

			// get file size
			fseek(file, 0, SEEK_END);
			long bytecodeLen = ftell(file);
			fseek(file, 0, SEEK_SET);

			// read bytecode
			char *bytecode = (char*)malloc(bytecodeLen);
			fread(bytecode, bytecodeLen, 1, file);

			// close file
			fclose(file);

			// create shader object
			HRESULT hr = wnd.GetDevice()->CreatePixelShader(bytecode, bytecodeLen, nullptr, (ID3D11PixelShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create pixel shader.\n");
				free(bytecode);
				return false;
			}

			// free memory
			free(bytecode);
		}

		return true;
	}
	bool PixelShader::LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile)
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
			HRESULT hr = D3DCompile(code, codeLen, "PixelShader", nullptr, nullptr, entry.c_str(), "ps_5_0", flags, 0, outCode.GetAddressOf(), outMsgs.GetAddressOf());
			if (FAILED(hr)) {
				printf("[D3D] Failed to compile the pixel shader");
				if (outMsgs != nullptr)
					printf(" (%s)", (char*)outMsgs->GetBufferPointer());
				printf(".\n");
				return false;
			}

			// create shader object with bytecode
			hr = wnd.GetDevice()->CreatePixelShader(outCode->GetBufferPointer(), outCode->GetBufferSize(), nullptr, (ID3D11PixelShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create pixel shader.\n");
				return false;
			}
		}
		else {
			// create shader object
			HRESULT hr = wnd.GetDevice()->CreatePixelShader(code, codeLen, nullptr, (ID3D11PixelShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create pixel shader.\n");
				return false;
			}
		}

		return true;
	}
	void PixelShader::Bind()
	{
		mWindow->GetDeviceContext()->PSSetShader((ID3D11PixelShader*)mShader, nullptr, 0);
	}
}