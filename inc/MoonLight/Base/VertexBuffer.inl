template<typename T>
inline void VertexBuffer<T>::Create(ml::Window& wnd, T* vert, ml::UInt32 vertCount, ml::Resource::Usage usg, ml::Resource::CPUAccess access)
{
	this->mWindow = &wnd;

	D3D11_BUFFER_DESC buffDesc;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.ByteWidth = sizeof(T)*vertCount;
	buffDesc.CPUAccessFlags = access;
	buffDesc.MiscFlags = 0;
	buffDesc.StructureByteStride = 0;
	buffDesc.Usage = (D3D11_USAGE)usg;

	if (vert != nullptr) {
		D3D11_SUBRESOURCE_DATA sub;
		sub.pSysMem = vert;
		sub.SysMemPitch = 0;
		sub.SysMemSlicePitch = 0;

		wnd.GetDevice()->CreateBuffer(&buffDesc, &sub, this->mBuffer.GetAddressOf());
	}
	else
		wnd.GetDevice()->CreateBuffer(&buffDesc, nullptr, this->mBuffer.GetAddressOf());
}

template<typename T>
inline void VertexBuffer<T>::Create(ml::Window& wnd, ml::UInt32 vertCount, ml::Resource::Usage usg, ml::Resource::CPUAccess access)
{
	this->Create(wnd, nullptr, vertCount, usg, access);
}

template<typename T>
inline void VertexBuffer<T>::Update(T* verts)
{
	this->mWindow->GetDeviceContext()->UpdateSubresource(this->mBuffer.Get(), 0, nullptr, verts, 0, 0);
}

template<typename T>
inline void VertexBuffer<T>::Bind(ml::UInt32 slot)
{
	UINT stride = sizeof(T);
	UINT offset = 0;

	this->mWindow->GetDeviceContext()->IASetVertexBuffers(slot, 1, this->mBuffer.GetAddressOf(), &stride, &offset);
}
