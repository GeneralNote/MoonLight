#include <MoonLight/Base/VertexShader.h>
#include <d3dcompiler.h>

namespace ml
{
	VertexShader::VertexShader() :
		Shader()
	{
		InputSignature = nullptr;
	}
	bool VertexShader::LoadFromFile(ml::Window& wnd, std::string filename, std::string entry, bool needsCompile)
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
			HRESULT hr = D3DCompileFromFile(wfilen.c_str(), nullptr, nullptr, entry.c_str(), "vs_5_0", flags, 0, outCode.GetAddressOf(), outMsgs.GetAddressOf());
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
		}
		else {
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
			HRESULT hr = wnd.GetDevice()->CreateVertexShader(bytecode, bytecodeLen, nullptr, (ID3D11VertexShader**)&mShader);
			if (FAILED(hr)) {
				printf("[D3D] Failed to create vertex shader.\n");
				free(bytecode);
				return false;
			}

			// free memory
			free(bytecode);

			// create input signature if its bound to this vertex shader
			mCreateInputSignature(wnd, bytecode, bytecodeLen);
		}

		return true;
	}
	bool VertexShader::LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile)
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
			HRESULT hr = D3DCompile(code, codeLen, "VertexShader", nullptr, nullptr, entry.c_str(), "vs_5_0", flags, 0, outCode.GetAddressOf(), outMsgs.GetAddressOf());
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
		}
		else {
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
		mWindow->GetDeviceContext()->VSSetShader((ID3D11VertexShader*)mShader, nullptr, 0);
	}
	void VertexShader::mCreateInputSignature(ml::Window& wnd, const char* bc, ml::UInt32 bcLen)
	{
		if (InputSignature != nullptr)
			if (InputSignature->GetInputLayout() == nullptr) // create the input signature only if needed
				InputSignature->Create(wnd, bc, bcLen);
	}
}