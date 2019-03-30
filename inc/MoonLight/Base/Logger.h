#pragma once
#include <string>

namespace ml
{
	class Logger
	{
	public:
		virtual void Log(const std::string& str);
	};
}