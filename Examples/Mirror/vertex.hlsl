cbuffer cbPerFrame : register(b0)
{
	matrix matVP;
	matrix matWorld;
	float Transparency;
};

struct VSInput
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float3 Normal : NORMAL;
};

static const float3 LightPos = float3(8.65, 3.42, 3.67);
static const float4 LightDiffuse = float4(0.5f, 0.5f, 0.5f, 1);
static const float4 mDiffuse = float4(0.8f, 0.8f, 0.8f, 1.0f);
static const float3 mAmbient = float3(0.3f, 0.3f, 0.3f);

VSOutput main(VSInput vin)
{
	VSOutput vout = (VSOutput)0;

	vout.Position = mul(mul(float4(vin.Position, 1.0f), matWorld), matVP);
	vout.Normal = vin.Normal;

	float3 worldPos = mul(vin.Position, (float3x3)matWorld);
	float3 worldNormal = normalize(mul(vin.Normal, (float3x3)matWorld));

	float3 lightVec = normalize(LightPos - worldPos);
	float diffuse = max(dot(worldNormal, lightVec), 0);

	vout.Color = float4(mAmbient, 0) + mDiffuse * LightDiffuse * diffuse;
	vout.Color.w = Transparency;

	return vout;
}