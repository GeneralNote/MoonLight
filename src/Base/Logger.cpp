#include <MoonLight/Base/Logger.h>
#include <stdio.h>

namespace ml
{
	void Logger::Log(const std::string& str)
	{
		printf("%s", str.c_str());
	}
}