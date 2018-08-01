
template<typename T>
inline void ConstantBuffer<T>::Create(ml::Window & wnd, T * data, ml::UInt32 dataSize, ml::Resource::Usage usage, ml::Resource::CPUAccess access)
{
	this->mWindow = &wnd;

	D3D11_BUFFER_DESC buffDesc;
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.ByteWidth = dataSize;
	buffDesc.CPUAccessFlags = (UINT)access;
	buffDesc.MiscFlags = 0;
	buffDesc.StructureByteStride = 0;
	buffDesc.Usage = (D3D11_USAGE)usage;

	if (data != nullptr) {
		D3D11_SUBRESOURCE_DATA sub;
		sub.pSysMem = data;
		sub.SysMemPitch = 0;
		sub.SysMemSlicePitch = 0;

		wnd.GetDevice()->CreateBuffer(&buffDesc, &sub, this->mBuffer.GetAddressOf());
	}
	else
		wnd.GetDevice()->CreateBuffer(&buffDesc, nullptr, this->mBuffer.GetAddressOf());
}

template<typename T>
inline void ConstantBuffer<T>::Create(ml::Window & wnd, ml::UInt32 dataSize, ml::Resource::Usage usg, ml::Resource::CPUAccess access)
{
	this->Create(wnd, nullptr, dataSize, usg, access);
}

template<typename T>
inline void ConstantBuffer<T>::Update(T * data)
{
	this->mWindow->GetDeviceContext()->UpdateSubresource(this->mBuffer.Get(), 0, nullptr, data, 0, 0);
}

template<typename T>
inline void ConstantBuffer<T>::BindVS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->VSSetConstantBuffers(slot, 1, this->mBuffer.GetAddressOf());
}

template<typename T>
inline void ConstantBuffer<T>::BindHS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->HSSetConstantBuffers(slot, 1, this->mBuffer.GetAddressOf());
}

template<typename T>
inline void ConstantBuffer<T>::BindDS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->DSSetConstantBuffers(slot, 1, this->mBuffer.GetAddressOf());
}

template<typename T>
inline void ConstantBuffer<T>::BindGS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->GSSetConstantBuffers(slot, 1, this->mBuffer.GetAddressOf());
}

template<typename T>
inline void ConstantBuffer<T>::BindPS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->PSSetConstantBuffers(slot, 1, this->mBuffer.GetAddressOf());
}

template<typename T>
inline void ConstantBuffer<T>::BindCS(ml::UInt32 slot)
{
	this->mWindow->GetDeviceContext()->CSSetConstantBuffers(slot, 1, this->mBuffer.GetAddressOf());
}
