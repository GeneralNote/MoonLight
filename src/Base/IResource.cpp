#include <MoonLight/Base/IResource.h>

namespace ml
{
	bool IResource::Map(MapData& data, IResource::MapType mapMode, ml::UInt32 resource, bool doNotWait)
	{
		assert(mWindow != nullptr);

		HRESULT hr = mWindow->GetDeviceContext()->Map(mResource.Get(), resource, (D3D11_MAP)mapMode, doNotWait * D3D11_MAP_FLAG_DO_NOT_WAIT, &data);
		m_lastResource = resource;

		return !FAILED(hr);
	}
	void IResource::Unmap()
	{
		assert(mWindow != nullptr);

		mWindow->GetDeviceContext()->Unmap(mResource.Get(), m_lastResource);
	}
}