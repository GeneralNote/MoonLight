#ifndef __MOONLIGHT_COMPUTESHADER_H__
#define __MOONLIGHT_COMPUTESHADER_H__

#include <MoonLight/Base/Shader.h>

namespace ml
{
	class ComputeShader : public Shader
	{
	public:
		virtual bool LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile = true, const ShaderMacroList& macros = ShaderMacroList());
		virtual void Bind();
	};
}

#endif // __MOONLIGHT_COMPUTESHADER_H__