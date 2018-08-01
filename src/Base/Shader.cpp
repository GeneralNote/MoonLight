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
	void Shader::mFree()
	{
		if (mShader != nullptr) {
			mShader->Release();
			mShader = nullptr;
		}
	}
}