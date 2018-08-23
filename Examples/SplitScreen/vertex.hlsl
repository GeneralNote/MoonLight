cbuffer cbPerFrame : register(b0)
{
	matrix matWVP;
	int viewport;
};

struct VSInput
{
	float3 Position : POSITION;
	float2 UV : TEXCOORD;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

VSOutput main( VSInput vin, out uint view : SV_ViewportArrayIndex )
{
	VSOutput vout = (VSOutput)0;

	vout.Position = mul(float4(vin.Position.xyz, 1.0f), matWVP);
	vout.UV = vin.UV;
	view = viewport;

	return vout;
}