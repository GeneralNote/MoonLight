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
	bool Shader::LoadFromFile(ml::Window & wnd, std::string filename, std::string entry, bool needsCompile, const ShaderMacroList& macros)
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
		bool ret = LoadFromMemory(wnd, bytecode, bytecodeLen, entry, needsCompile, macros);

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

	ShaderMacroList::ShaderMacroList()
	{
		mMacros.push_back({ nullptr, nullptr });
	}

	ShaderMacroList::~ShaderMacroList()
	{
		for (int i = 0; i < mMacros.size(); i++) {
			if (mMacros[i].Name != nullptr) delete mMacros[i].Name;
			if (mMacros[i].Definition != nullptr) delete mMacros[i].Definition;
			mMacros[i].Name = nullptr;
			mMacros[i].Definition = nullptr;
		}
	}

	void ShaderMacroList::Add(const std::string & name, const std::string & value)
	{
		mMacros[mMacros.size() - 1].Name = new char[name.size() + 1];
		mMacros[mMacros.size() - 1].Definition = new char[value.size() + 1];

		memcpy((void*)mMacros[mMacros.size() - 1].Name, name.c_str(), name.size() * sizeof(char));
		memcpy((void*)mMacros[mMacros.size() - 1].Definition, value.c_str(), value.size() * sizeof(char));

		mMacros.push_back({ nullptr, nullptr });
	}
}