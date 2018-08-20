template<typename T>
inline void ml::GenericBuffer<T>::Create(ml::Window& wnd, T* vert, ml::UInt32 vertCount, ml::Resource::Flags flags)
{
	this->mWindow = &wnd;

	D3D11_BUFFER_DESC buffDesc;
	buffDesc.ByteWidth = sizeof(T)*vertCount;
	buffDesc.StructureByteStride = sizeof(T);

	ml::Resource::Options::Parse(flags, (UINT32&)buffDesc.Usage, buffDesc.CPUAccessFlags, buffDesc.BindFlags, buffDesc.MiscFlags);
	
	if (vert != nullptr) {
		D3D11_SUBRESOURCE_DATA sub;
		sub.pSysMem = vert;
		sub.SysMemPitch = 0;
		sub.SysMemSlicePitch = 0;

		wnd.GetDevice()->CreateBuffer(&buffDesc, &sub, this->mBuffer.GetAddressOf());
	} else
		wnd.GetDevice()->CreateBuffer(&buffDesc, nullptr, this->mBuffer.GetAddressOf());
}

template<typename T>
inline void ml::GenericBuffer<T>::Create(ml::Window& wnd, ml::UInt32 vertCount, ml::Resource::Flags flags)
{
	this->Create(wnd, nullptr, vertCount, flags);
}

template<typename T>
inline void ml::GenericBuffer<T>::Update(T* verts)
{
	this->mWindow->GetDeviceContext()->UpdateSubresource(this->mBuffer.Get(), 0, nullptr, verts, 0, 0);
}