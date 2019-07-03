#include <MoonLight/Base/Texture.h>

namespace ml
{
	bool Texture::Create(ml::Window& wnd, ml::Image & img, ml::Resource::Flags flags)
	{
		this->SetOwner(wnd);
		mResource.Reset();

		// parse the flags
		UInt32 usage, bind, access, misc;
		ml::Resource::Options::Parse(flags, usage, access, bind, misc);

		// create texture
		HRESULT hr = DirectX::CreateTextureEx(wnd.GetDevice(), img.GetImage()->GetImages(),
			img.GetImage()->GetImageCount(), img.GetImage()->GetMetadata(), (D3D11_USAGE)usage, bind, access,
			misc, false, mResource.GetAddressOf());
		
		return !FAILED(hr);
	}
	bool Texture::Create(ml::Window& wnd, int width, int height, ml::Resource::Flags flags, DXGI_FORMAT fmt)
	{
		this->SetOwner(wnd);
		mResource.Reset();

		// get the flags
		UInt32 usage, bind, access, misc;
		ml::Resource::Options::Parse(flags, usage, access, bind, misc);

		// create an empty pink texture
		uint8_t bytesPerPixel = std::max<uint8_t>(1, DirectX::BitsPerPixel(fmt) / 8);
		const int imageSize = width * height * bytesPerPixel;
		uint8_t* empty = new uint8_t[imageSize];
		memset(empty, 0xFF, sizeof(uint8_t) * imageSize);

		// create texture
		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = empty;
		data.SysMemPitch = width * bytesPerPixel;
		data.SysMemSlicePitch = 0;
		
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = desc.ArraySize = 1;
		desc.Format = fmt;
		desc.SampleDesc.Count = 1;
		desc.Usage = (D3D11_USAGE)usage;
		desc.BindFlags = bind;
		desc.CPUAccessFlags = access;
		desc.MiscFlags = misc;

		HRESULT hr = wnd.GetDevice()->CreateTexture2D(&desc, &data, (ID3D11Texture2D**)mResource.GetAddressOf());

		delete[] empty;

		if (FAILED(hr))
			return false;
		return true;
	}
}