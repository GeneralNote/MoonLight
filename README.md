# MOONLIGHT
Simple Direct3D 11 wrapper.
![Animation](https://i.imgur.com/N59IGah.gif)

## TODO:
- [x] ml::Timer
- [ ] Viewport
- [ ] StructuredBuffer<T\>, Buffer<T\>, Consume/AppendStructuredBuffer
- [ ] UnorderedAccessView + ShaderResourceView (*RW*Texture2D, *RW*StructuredBuffer, *RW*Buffer)
- [x] ComputeShader
- [x] ml\::Texture, ml::Image
- [ ] assert(mWnd != nullptr)
- [x] multiple file formats (support .dds)
- [ ] Texture arrays
- [ ] ml::Logger & check for HRESULTs
- [ ] convert ml\::Resource::CPUAccess parameters to UInt8 so that bitwise operators can be used
- [ ] make Shader class implement LoadFromFile which calls LoadFromMemory
- [ ] set debug object names in debug mode ( SetPrivateData(WKPDID_D3DDebugObjectName) )
- [ ] create batch classes (ml::VertexBufferBatch, ml::ConstantBufferBatch, ml::ViewportBatch, etc..)
- [ ] shader include handler - handles #include statements in loaded .hlsl files ( ml::ShaderIncludeHandler )
- [ ] shader macro definitions ( ml::ShaderMacro sMacros; sMacros.Add("QUALITY", "1+4"); shader.SetMacros(sMacros); )
- [ ] vertex buffer input slots ( ml::VertexBufferBatch batch; batch.Add(positionBuffer); batch.Add(normalBuffer); batch.Bind(); )
