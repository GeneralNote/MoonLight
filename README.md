# Moonlight
Simple Direct3D 11 wrapper.
![Animation](https://i.imgur.com/N59IGah.gif)

## TODO:
- [ ] OpenGL support + Linux
- [ ] Texture arrays
- [ ] check for HRESULTs
- [ ] set debug object names in debug mode ( SetPrivateData(WKPDID_D3DDebugObjectName) )
- [ ] create batch classes (ml\::VertexBufferBatch, ml\::ConstantBufferBatch, ml\::ViewportBatch, etc..)
- [ ] vertex buffer input slots ( ml::VertexBufferBatch batch; batch.Add(positionBuffer); batch.Add(normalBuffer); batch.Bind(); )
- [ ] better and more attractive compute shader example
- [ ] ml::Renderer -> ml\::WindowRenderer implements ID3D11DeviceContext, ml\::ThreadedRenderer implements ID3D11DefferedContext -> BindVS(myRenderer, slotNumber)
- [ ] rename ml\::Topology into ml\::PrimitiveType
- [ ] change style from mName to m_name
- [ ] universal header: #inclued <MoonLight.hpp>
- [ ] QOL: IndexBuffer + example