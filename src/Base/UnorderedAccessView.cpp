#include <MoonLight/Base/UnorderedAccessView.h>

namespace ml
{
	bool UnorderedAccessView::Create(ml::Window & wnd, ml::Texture & tex)
	{
		mView.Reset();

		HRESULT hr = wnd.GetDevice()->CreateUnorderedAccessView(tex.GetResource(), nullptr, mView.GetAddressOf());
		mWnd = &wnd;

		return !FAILED(hr);
	}
	void UnorderedAccessView::Bind(ml::UInt32 slot)
	{
		assert(mWnd != nullptr);

		mWnd->GetDeviceContext()->CSSetUnorderedAccessViews(slot, 1, mView.GetAddressOf(), nullptr);
	}
}