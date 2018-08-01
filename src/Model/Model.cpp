#include <MoonLight/Model/Model.h>

namespace ml
{
	bool Model::LoadFromFile(const std::string& filename)
	{
		// open file
		FILE *file = fopen(filename.c_str(), "rb");

		if (file == nullptr)
			return false;

		// get file size
		fseek(file, 0, SEEK_END);
		long bytecodeLen = ftell(file);
		fseek(file, 0, SEEK_SET);

		// read bytecode
		char *bytecode = (char*)malloc(bytecodeLen);
		fread(bytecode, bytecodeLen, 1, file);

		// close file
		fclose(file);

		// actually parse the data
		bool ret = LoadFromMemory(bytecode, bytecodeLen);

		// free the memory
		free(bytecode);

		return ret;
	}
}
