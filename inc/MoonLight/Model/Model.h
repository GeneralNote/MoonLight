#ifndef __MOONLIGHT_MODEL_H__
#define __MOONLIGHT_MODEL_H__

#include <MoonLight/Base/Math.h>
#include <string>

namespace ml
{
	class Model
	{
	public:
		// load from file
		virtual bool LoadFromFile(const std::string& filename);

		// load from memory
		virtual bool LoadFromMemory(const char* data, ml::UInt32 dataLen) = 0;
	};
}

#endif // __MOONLIGHT_MODEL_H__