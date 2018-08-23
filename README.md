# MOONLIGHT
Simple Direct3D 11 wrapper.
![Animation](https://i.imgur.com/N59IGah.gif)

## TODO:
- [x] ml::Timer
- [ ] Viewport
- [x] StructuredBuffer<T\>, Buffer<T\>
- [x] UnorderedAccessView + ShaderResourceView (*RW*Texture2D, *RW*StructuredBuffer, *RW*Buffer)
- [x] ComputeShader
- [x] ml\::Texture, ml::Image
- [x] assert(mWnd != nullptr)
- [x] multiple file formats (support .dds)
- [ ] Texture arrays
- [ ] Map() and Unmap() for ID3D11Resource (implement IResource)
- [ ] Buffer\<T\> - some methods are inline and they shouldnt be (and the other way around)
- [ ] ml::Logger & check for HRESULTs
- [ ] ml::RenderTexture
- [ ] ml::ShaderVersion
- [ ] ml::GeometryGenerator -> create spheres, planes, cubes, etc... at run time
- [x] convert ml\::Resource::CPUAccess parameters to UInt8 so that bitwise operators can be used
- [x] make Shader class implement LoadFromFile which calls LoadFromMemory
- [ ] set debug object names in debug mode ( SetPrivateData(WKPDID_D3DDebugObjectName) )
- [ ] create batch classes (ml\::VertexBufferBatch, ml\::ConstantBufferBatch, ml\::ViewportBatch, etc..)
- [ ] shader include handler - handles #include statements in loaded .hlsl files ( ml::ShaderIncludeHandler )
- [ ] shader macro definitions ( ml::ShaderMacro sMacros; sMacros.Add("QUALITY", "1+4"); shader.SetMacros(sMacros); )
- [ ] vertex buffer input slots ( ml::VertexBufferBatch batch; batch.Add(positionBuffer); batch.Add(normalBuffer); batch.Bind(); )
- [ ] better and more attractive compute shader example
- [ ] ml::Renderer -> ml\::WindowRenderer implements ID3D11DeviceContext, ml\::ThreadedRenderer implements ID3D11DefferedContext -> BindVS(myRenderer, slotNumber)
- [ ] rename ml\::Topology into ml\::PrimitiveType
- [ ] change style from mName to m_name
- [ ] universal header: #inclued <MoonLight.hpp>
- [ ] IndexBuffer + example