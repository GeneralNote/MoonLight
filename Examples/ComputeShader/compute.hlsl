struct Vector3D
{
	float3 Position;
};

StructuredBuffer<Vector3D> vectorBuffer : register(t0);
RWBuffer<float> lengthBuffer : register(u0);

[numthreads(64,1,1)]
void main(in uint3 id : SV_DispatchThreadID)
{
	float3 pos = vectorBuffer[id.x].Position;
	lengthBuffer[id.x] = sqrt(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z);
}