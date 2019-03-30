#include <MoonLight/Base/PixelShader.h>
#include <d3dcompiler.h>

namespace ml
{
	bool PixelShader::LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile, const Shader::MacroList& macros, const IncludeHandler& include)
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
			HRESULT hr = D3DCompile(code, codeLen, mFile.c_str(), macros.GetData(), &hInclude, entry.c_str(), "ps_5_0", flags, 0, outCode.GetAddressOf(), outMsgs.GetAddressOf());
			if (FAILED(hr)) {
				wnd.GetLogger()->Log("[D3D] Failed to compile the pixel shader.");
				if (outMsgs != nullptr)
					wnd.GetLogger()->Log((char*)outMsgs->GetBufferPointer());
				return false;
			}

			// create shader object with bytecode
			hr = wnd.GetDevice()->CreatePixelShader(outCode->GetBufferPointer(), outCode->GetBufferSize(), nullptr, (ID3D11PixelShader**)&mShader);
			if (FAILED(hr)) {
				wnd.GetLogger()->Log("[D3D] Failed to create pixel shader.");
				return false;
			}
		} else {
			// create shader object
			HRESULT hr = wnd.GetDevice()->CreatePixelShader(code, codeLen, nullptr, (ID3D11PixelShader**)&mShader);
			if (FAILED(hr)) {
				wnd.GetLogger()->Log("[D3D] Failed to create pixel shader.");
				return false;
			}
		}

		return true;
	}
	void PixelShader::Bind()
	{
		assert(mWindow != nullptr);

		mWindow->GetDeviceContext()->PSSetShader((ID3D11PixelShader*)mShader, nullptr, 0);
	}
}