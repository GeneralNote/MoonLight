#ifndef __MOONLIGHT_IMAGE_H__
#define __MOONLIGHT_IMAGE_H__

#include <MoonLight/Base/Math.h>
#include <DirectXTex.h>

namespace ml
{
	class Image
	{
	public:
		Image();
		~Image();

		bool LoadFromFile(const std::string& fname);

		bool LoadFromMemory(const char* data, ml::UInt32 dataLen);

		bool GenerateMipMap(size_t levels);

		inline DirectX::ScratchImage* GetImage() { return mData; }

	private:
		DirectX::ScratchImage* mData;
	};
}

#endif // __MOONLIGHT_IMAGE_H__