struct PSInput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState smp : register(s0);

static const float4 snowflakeColor = float4(1, 1, 1, 1);

float4 main(PSInput pin) : SV_TARGET
{
	return snowflakeColor * tex.Sample(smp, pin.UV);
}