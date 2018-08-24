
template<typename T>
inline void ml::ConstantBuffer<T>::Create(ml::Window & wnd, T * data, ml::UInt32 dataSize, ml::Resource::Flags flags)
{
	this->SetOwner(wnd);

	D3D11_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth = dataSize;
	buffDesc.StructureByteStride = 0;

	ml::Resource::Options::Parse(flags, (UINT32&)buffDesc.Usage, buffDesc.CPUAccessFlags, buffDesc.BindFlags, buffDesc.MiscFlags);
	buffDesc.BindFlags |= D3D11_BIND_CONSTANT_BUFFER;

	if (data != nullptr) {
		D3D11_SUBRESOURCE_DATA sub;
		sub.pSysMem = data;
		sub.SysMemPitch = 0;
		sub.SysMemSlicePitch = 0;

		wnd.GetDevice()->CreateBuffer(&buffDesc, &sub, (ID3D11Buffer**)this->mResource.GetAddressOf());
	}
	else
		wnd.GetDevice()->CreateBuffer(&buffDesc, nullptr, (ID3D11Buffer**)this->mResource.GetAddressOf());
}

template<typename T>
inline void ml::ConstantBuffer<T>::Create(ml::Window & wnd, ml::UInt32 dataSize, ml::Resource::Flags flags)
{
	this->Create(wnd, nullptr, dataSize, flags);
}

template<typename T>
inline void ml::ConstantBuffer<T>::Update(T * data)
{
	this->mWindow->GetDeviceContext()->UpdateSubresource(this->mResource.Get(), 0, nullptr, data, 0, 0);
}

template<typename T>
inline void ml::ConstantBuffer<T>::BindVS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->VSSetConstantBuffers(slot, 1, (ID3D11Buffer**)this->mResource.GetAddressOf());
}

template<typename T>
inline void ml::ConstantBuffer<T>::BindHS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->HSSetConstantBuffers(slot, 1, (ID3D11Buffer**)this->mResource.GetAddressOf());
}

template<typename T>
inline void ml::ConstantBuffer<T>::BindDS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->DSSetConstantBuffers(slot, 1, (ID3D11Buffer**)this->mResource.GetAddressOf());
}

template<typename T>
inline void ml::ConstantBuffer<T>::BindGS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->GSSetConstantBuffers(slot, 1, (ID3D11Buffer**)this->mResource.GetAddressOf());
}

template<typename T>
inline void ml::ConstantBuffer<T>::BindPS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->PSSetConstantBuffers(slot, 1, (ID3D11Buffer**)this->mResource.GetAddressOf());
}

template<typename T>
inline void ml::ConstantBuffer<T>::BindCS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->CSSetConstantBuffers(slot, 1, (ID3D11Buffer**)this->mResource.GetAddressOf());
}
