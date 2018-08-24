#ifndef __MOONLIGHT_IRESOURCE_H__
#define __MOONLIGHT_IRESOURCE_H__

#include <MoonLight/Base/Ptr.h>
#include <MoonLight/Base/Window.h>

namespace ml
{
	class IResource
	{
	public:
		enum class MapType
		{
			Read = D3D11_MAP_READ,
			Write = D3D11_MAP_WRITE,
			ReadWrite = D3D11_MAP_READ_WRITE,
			WriteDiscard = D3D11_MAP_WRITE_DISCARD,
			WriteNoOverwrite = D3D11_MAP_WRITE_NO_OVERWRITE
		};
		typedef D3D11_MAPPED_SUBRESOURCE MapData;

		IResource() { mResource = nullptr; mWindow = nullptr; m_lastResource = 0; }

		inline ID3D11Resource* GetResource() { return mResource.Get(); }
		bool Map(MapData& data, MapType mapMode, ml::UInt32 resource = 0, bool doNotWait = false);
		void Unmap();

	protected:
		inline void SetOwner(ml::Window& wnd) { mWindow = &wnd; }

		ml::UInt32 m_lastResource;
		ml::Window* mWindow;
		ml::Ptr<ID3D11Resource> mResource;
	};
}

#endif // __MOONLIGHT_IRESOURCE_H__