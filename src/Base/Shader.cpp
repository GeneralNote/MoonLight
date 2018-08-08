#include <MoonLight/Base/Shader.h>

namespace ml
{
	Shader::Shader()
	{
		mShader = nullptr;
		mWindow = nullptr;
	}
	Shader::~Shader()
	{
		mFree();
	}
	bool Shader::LoadFromFile(ml::Window & wnd, std::string filename, std::string entry, bool needsCompile)
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
		bool ret = LoadFromMemory(wnd, bytecode, bytecodeLen, entry, needsCompile);

		// free the memory
		free(bytecode);

		return ret;
	}
	void Shader::mFree()
	{
		if (mShader != nullptr) {
			mShader->Release();
			mShader = nullptr;
		}
	}
}