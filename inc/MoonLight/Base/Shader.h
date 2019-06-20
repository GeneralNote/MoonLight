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
		class MacroList
		{
		public:
			MacroList();
			~MacroList();

			void Add(const std::string& name, const std::string& value);
			inline const D3D_SHADER_MACRO* GetData() const { return mMacros.data(); }

		private:
			std::vector<D3D_SHADER_MACRO> mMacros;
		};
		class IncludeHandler
		{
		public:
			enum class IncludeType
			{
				Local,
				System
			};
			virtual bool Open(const std::string& filename, IncludeType type, const void** outData, UINT* outBytes);
			virtual void Close(const void* data);
		};

		Shader();
		~Shader();

		bool LoadFromFile(ml::Window& wnd, std::string filename, std::string entry, bool needsCompile = true, const Shader::MacroList& macros = Shader::MacroList(), const IncludeHandler& include = IncludeHandler());
		virtual bool LoadFromMemory(ml::Window& wnd, const char* code, ml::UInt32 codeLen, std::string entry, bool needsCompile = true, const Shader::MacroList& macros = Shader::MacroList(), const IncludeHandler& include = IncludeHandler()) = 0;
		virtual void Bind() = 0;

		inline ID3D11DeviceChild* GetShader() { return mShader; }

	protected:
		void mFree();

		std::string mFile;

		ID3D11DeviceChild* mShader;
		ml::Window* mWindow;
	};

	namespace priv_impl
	{
		// the actual shader include handler
		class ID3DIncludeHandler : public ID3DInclude
		{
		public:
			ID3DIncludeHandler();
			ID3DIncludeHandler(Shader::IncludeHandler* handle);

			HRESULT __stdcall Open(D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
			HRESULT __stdcall Close(LPCVOID pData);

			Shader::IncludeHandler* Handle;
		};
	}
}

#endif // __MOONLIGHT_SHADER_H__