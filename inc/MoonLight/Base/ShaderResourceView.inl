template<typename T>
inline bool ml::ShaderResourceView::Create(ml::Window & wnd, ml::Buffer<T>& buffer)
{
	mView.Reset();

	// TODO: we probably have to provide SHADER_RESOURCE_VIEW_DESC for this to work
	HRESULT hr = wnd.GetDevice()->CreateShaderResourceView(buffer.GetBuffer(), nullptr, mView.GetAddressOf());
	mWnd = &wnd;

	return !FAILED(hr);
}