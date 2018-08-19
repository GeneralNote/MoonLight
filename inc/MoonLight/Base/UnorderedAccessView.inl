template<typename T>
inline bool ml::UnorderedAccessView::Create(ml::Window & wnd, ml::Buffer<T>& buffer)
{
	mView.Reset();

	D3D11_BUFFER_DESC bufDesc;
	buffer.GetBuffer()->GetDesc(&bufDesc);

	bool isRawView = bufDesc.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = (DXGI_FORMAT)((DXGI_FORMAT_UNKNOWN * !isRawView) + (DXGI_FORMAT_R32_TYPELESS * isRawView));
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW * isRawView;
	uavDesc.Buffer.FirstElement = 0;
	uavDesc.Buffer.NumElements = bufDesc.ByteWidth/sizeof(T);

	HRESULT hr = wnd.GetDevice()->CreateUnorderedAccessView(buffer.GetBuffer(), &uavDesc, mView.GetAddressOf());
	mWnd = &wnd;

	return !FAILED(hr);
}