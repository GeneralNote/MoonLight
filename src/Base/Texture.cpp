#include <MoonLight/Base/Texture.h>

namespace ml
{
	bool Texture::Create(ml::Window& wnd, ml::Image & img, ml::UInt32 bind, bool cubemap, ml::Resource::Usage usg, ml::UInt32 access)
	{
		mTexture.Reset();

		UINT srv = (bind & Texture::Bind::ShaderResource) * D3D11_BIND_SHADER_RESOURCE;
		UINT rtv = (bind & Texture::Bind::RenderTarget) * D3D11_BIND_RENDER_TARGET;

		HRESULT hr = DirectX::CreateTextureEx(wnd.GetDevice(), img.GetImage()->GetImages(),
			img.GetImage()->GetImageCount(), img.GetImage()->GetMetadata(), (D3D11_USAGE)usg, srv | rtv, access,
			cubemap * D3D11_RESOURCE_MISC_TEXTURECUBE, false, reinterpret_cast<ID3D11Resource**>(mTexture.GetAddressOf()));
		
		return !FAILED(hr);
	}
}