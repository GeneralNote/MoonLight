struct Vertex
{
	float3 Position : POSITION;
};

RWByteAddressBuffer vertexBuffer : register(u0);

[numthreads(8,8,1)]
void main(uint id : SV_GroupIndex)
{
	//vertexBuffer.Store3(id * 12, uint3(0, 0, 0));
	//vertexBuffer[id].Position = float3(0, 0, 0);
}