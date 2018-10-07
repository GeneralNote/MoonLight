#include <MoonLight/Base/Shader.h>
#include <fstream>

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
	bool Shader::LoadFromFile(ml::Window & wnd, std::string filename, std::string entry, bool needsCompile, const Shader::MacroList& macros, const IncludeHandler& include)
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
		bool ret = LoadFromMemory(wnd, bytecode, bytecodeLen, entry, needsCompile, macros, include);

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

	Shader::MacroList::MacroList()
	{
		mMacros.push_back({ nullptr, nullptr });
	}
	Shader::MacroList::~MacroList()
	{
		for (int i = 0; i < mMacros.size(); i++) {
			if (mMacros[i].Name != nullptr) delete mMacros[i].Name;
			if (mMacros[i].Definition != nullptr) delete mMacros[i].Definition;
			mMacros[i].Name = nullptr;
			mMacros[i].Definition = nullptr;
		}
	}
	void Shader::MacroList::Add(const std::string & name, const std::string & value)
	{
		mMacros[mMacros.size() - 1].Name = new char[name.size() + 1];
		mMacros[mMacros.size() - 1].Definition = new char[value.size() + 1];

		memcpy((void*)mMacros[mMacros.size() - 1].Name, name.c_str(), name.size() * sizeof(char));
		memcpy((void*)mMacros[mMacros.size() - 1].Definition, value.c_str(), value.size() * sizeof(char));

		mMacros.push_back({ nullptr, nullptr });
	}

	namespace priv_impl
	{
		ID3DIncludeHandler::ID3DIncludeHandler() : Handle(nullptr) {}
		ID3DIncludeHandler::ID3DIncludeHandler(Shader::IncludeHandler * handle) : Handle(handle) {}
		HRESULT __stdcall ID3DIncludeHandler::Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID * ppData, UINT * pBytes)
		{
			assert(Handle != nullptr);

			bool ret = Handle->Open(std::string(pFileName), (ml::Shader::IncludeHandler::IncludeType)IncludeType, ppData, pBytes);

			return ret ? S_OK : E_FAIL;
		}
		HRESULT __stdcall ID3DIncludeHandler::Close(LPCVOID pData)
		{
			assert(Handle != nullptr);

			Handle->Close(pData);

			return S_OK;
		}
	}
	bool Shader::IncludeHandler::Open(const std::string & filename, IncludeType type, const void ** outData, UINT * outBytes)
	{
		std::ifstream fileData(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

		if (fileData.is_open()) {
			*outBytes = fileData.tellg();
			*outData = new char[*outBytes];
			fileData.seekg(0, std::ios::beg);
			fileData.read((char*)(*outData), *outBytes);
			fileData.close();
		} else return false;

		return true;
	}
	void Shader::IncludeHandler::Close(const void * data)
	{
		delete[] data;
	}
}

