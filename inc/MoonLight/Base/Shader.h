#ifndef __MOONLIGHT_SHADER_H__
#define __MOONLIGHT_SHADER_H__
#include <MoonLight/Base/Window.h>
#include <MoonLight/Base/Ptr.h>
#include <d3d11.h>
#include <string>

namespace ml
{
	class Shader
	{
	public:
		Shader();
		~Shader();

		virtual bool LoadFromFile(ml::Window& wnd, std::string filename, std::string entry, bool needsCompile = true) = 0;
		virtual bool LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile = true) = 0;
		virtual void Bind() = 0;

	protected:
		void mFree();

		ID3D11DeviceChild* mShader;
		ml::Window* mWindow;
	};
}

#endif // __MOONLIGHT_SHADER_H__