template<typename T>
inline bool ml::ShaderResourceView::Create(ml::Window & wnd, ml::Buffer<T>& buffer, DXGI_FORMAT fmt)
{
	mView.Reset();

	D3D11_BUFFER_DESC bufDesc;
	buffer.GetBuffer()->GetDesc(&bufDesc);

	bool isRawView = bufDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = (DXGI_FORMAT)((fmt * !isRawView) + (DXGI_FORMAT_R32_TYPELESS * isRawView));
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	srvDesc.BufferEx.Flags = D3D11_BUFFER_UAV_FLAG_RAW * isRawView;
	srvDesc.Buffer.FirstElement = srvDesc.BufferEx.FirstElement = 0;
	srvDesc.Buffer.NumElements = srvDesc.BufferEx.NumElements = bufDesc.ByteWidth / sizeof(T);

	HRESULT hr = wnd.GetDevice()->CreateShaderResourceView(buffer.GetBuffer(), &srvDesc, mView.GetAddressOf());
	mWnd = &wnd;

	return !FAILED(hr);
}