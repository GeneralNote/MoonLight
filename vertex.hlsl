cbuffer cbPerFrame : register(b0)
{
	matrix matWVP;
	matrix matWorld;
};
cbuffer cbMaterial : register(b1)
{
	float4 mDiffuse;
	float4 mSpecular;
	float3 mAmbient;
};

struct VSInput
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD;
};

static const float3 LightPos = float3(50, 50, 50);
static const float3 LightDiffuse = float3(1.0f, 1.0f, 1.0f);

VSOutput main(VSInput vin)
{
	VSOutput vout = (VSOutput)0;

	vout.Position = mul(float4(vin.Position, 1.0f), matWVP);
	vout.Normal = vin.Normal;

	float3 worldPos = mul(vin.Position, (float3x3)matWorld);
	float3 worldNormal = normalize(mul(vin.Normal, (float3x3)matWorld));

	float3 lightVec = normalize(LightPos - worldPos);
	float diffuse = max(dot(worldNormal, lightVec), 0);

	vout.Color.xyz = mAmbient + mDiffuse.xyz * LightDiffuse * diffuse;
	vout.Color.w = mDiffuse.w;

	vout.UV = vin.UV;

	return vout;
}