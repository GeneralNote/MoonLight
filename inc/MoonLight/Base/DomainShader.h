#ifndef __MOONLIGHT_DOMAINSHADER_H__
#define __MOONLIGHT_DOMAINSHADER_H__

#include <MoonLight/Base/Shader.h>

namespace ml
{
	class DomainShader : public Shader
	{
	public:
		virtual bool LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile = true, const Shader::MacroList& macros = Shader::MacroList(), const IncludeHandler& include = IncludeHandler());
		virtual void Bind();
	};
}

#endif // __MOONLIGHT_DOMAINSHADER_H__