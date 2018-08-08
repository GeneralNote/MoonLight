#include <MoonLight/Base/Image.h>

namespace ml
{
	Image::Image()
	{
		mData = new DirectX::ScratchImage();
	}
	Image::~Image()
	{
		mData->Release();
		delete mData;
	}
	bool Image::LoadFromFile(const std::string & fname)
	{
		
		Image::Type type = Image::Type::WIC;

		std::size_t lastDot = fname.find_last_of('.');
		std::string ext = fname.substr(lastDot + 1, fname.size() - (lastDot + 1));

		std::transform(ext.begin(), ext.end(), ext.begin(), tolower);

		if (ext == "dds")
			type = Image::Type::DDS;
		else if (ext == "tga")
			type = Image::Type::TGA;
		else if (ext == "hdr")
			type = Image::Type::HDR;

		return LoadFromFile(fname, type);
	}
	bool Image::LoadFromFile(const std::string & fname, Type type)
	{
		if (mData != nullptr)
			mData->Release();

		// open file
		FILE *file = fopen(fname.c_str(), "rb");

		if (file == nullptr)
			return false;

		// get file size
		fseek(file, 0, SEEK_END);
		long dataLen = ftell(file);
		fseek(file, 0, SEEK_SET);

		// read bytecode
		char *data = (char*)malloc(dataLen);
		fread(data, dataLen, 1, file);

		// close file
		fclose(file);

		bool ret = LoadFromMemory(data, dataLen, type);

		// free memory
		free(data);

		return ret;
	}
	bool Image::LoadFromMemory(const char * data, ml::UInt32 dataLen, Type type)
	{
		DirectX::TexMetadata meta;
		HRESULT hr = S_OK;
		if (type == Type::DDS)
			hr = DirectX::LoadFromDDSMemory(data, dataLen, 0, &meta, *mData);
		else if (type == Type::HDR)
			hr = DirectX::LoadFromHDRMemory(data, dataLen, &meta, *mData);
		else if (type == Type::TGA)
			hr = DirectX::LoadFromTGAMemory(data, dataLen, &meta, *mData);
		else
			hr = DirectX::LoadFromWICMemory(data, dataLen, 0, &meta, *mData);

		return !FAILED(hr);
	}
	bool Image::GenerateMipMap(size_t levels)
	{
		DirectX::ScratchImage* tempImg = new DirectX::ScratchImage();

		HRESULT hr = DirectX::GenerateMipMaps(mData->GetImages(), mData->GetImageCount(), mData->GetMetadata(),
			0, levels, *tempImg);

		// copy new image
		if (mData != nullptr) {
			mData->Release();
			delete mData;
		}

		mData = tempImg;

		return !FAILED(hr);
	}
}