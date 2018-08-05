# MOONLIGHT
Simple Direct3D 11 wrapper.
![Animation](https://i.imgur.com/FQnkPJ0.gif)

NOTE: the rotor movement is choppy only on the GIF. The FPS didnt sync, the program was running at 8K FPS while recording is at 24fps.

## TODO:
- [x] ml::Timer
- [ ] Viewport
- [x] ml\::Texture, ml::Image
- [ ] ml::Logger & check for HRESULTs
- [ ] convert ml\::Resource::CPUAccess parameters to UInt8 so that bitwise operators can be used
- [ ] make Shader class implement LoadFromFile which calls LoadFromMemory
- [ ] set debug object names in debug mode ( SetPrivateData(WKPDID_D3DDebugObjectName) )
- [ ] create batch classes (ml::VertexBufferBatch, ml::ConstantBufferBatch, ml::ViewportBatch, etc..)
- [ ] shader include handler - handles #include statements in loaded .hlsl files ( ml::ShaderIncludeHandler )
- [ ] shader macro definitions ( ml::ShaderMacro sMacros; sMacros.Add("QUALITY", "1+4"); shader.SetMacros(sMacros); )
- [ ] vertex buffer input slots ( ml::VertexBufferBatch batch; batch.Add(positionBuffer); batch.Add(normalBuffer); batch.Bind(); )
