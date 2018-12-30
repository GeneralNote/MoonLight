#ifndef __MOONLIGHT_IMAGE_H__
#define __MOONLIGHT_IMAGE_H__

#include <MoonLight/Base/Math.h>
#include <DirectXTex.h>
#include <string>

namespace ml
{
	class Image
	{
	public:
		enum class Type
		{
			DDS,
			HDR,
			TGA,
			WIC
		};

		Image();
		~Image();

		bool LoadFromFile(const std::string& fname);

		bool LoadFromFile(const std::string& fname, Type type);

		bool LoadFromMemory(const char* data, ml::UInt32 dataLen, Type type);

		bool GenerateMipMap(size_t levels);

		inline DirectX::ScratchImage* GetImage() { return mData; }

	private:
		DirectX::ScratchImage* mData;
	};
}

#endif // __MOONLIGHT_IMAGE_H__