#include <MoonLight/Base/Texture.h>

namespace ml
{
	bool Texture::Create(ml::Window& wnd, ml::Image & img, ml::Resource::Flags flags)
	{
		mTexture.Reset();

		// parse the flags
		UInt32 usage, bind, access, misc;
		ml::Resource::Options::Parse(flags, usage, access, bind, misc);

		// create texture
		HRESULT hr = DirectX::CreateTextureEx(wnd.GetDevice(), img.GetImage()->GetImages(),
			img.GetImage()->GetImageCount(), img.GetImage()->GetMetadata(), (D3D11_USAGE)usage, bind, access,
			misc, false, reinterpret_cast<ID3D11Resource**>(mTexture.GetAddressOf()));
		
		return !FAILED(hr);
	}
}