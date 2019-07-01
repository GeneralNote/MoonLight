# Moonlight
MoonLight is a simple **Direct3D 11 wrapper**. It exists to lower the number of lines
needed to write a Direct3D app. In future I might add OpenGL and Linux support.

## Building
Before building, you should install DirectXTex via `vcpkg`. If you dont use
`vcpkg` then you should link DirectXTex manually by compiling the [source](https://github.com/microsoft/directxtex)
and linking it to this project. Building DirectXTex is also straightforward -
you only need to open the `.sln` file provided in their repo and hit build.

To build **MoonLight** open `.sln` file and build the solution. It is set up to build
a `.lib` file by default.

## Features
Currently it has almost no additional features. It is currently only a Direct3D wrapper. But
it can also load .obj files and textures which is not built into the Direct3D 11. It alos might
not have a class or function for some of Direct3D functionalities.

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

## LICENSE
MoonLight is licensed under MIT license. See [LICENSE](./LICENSE) for more details.