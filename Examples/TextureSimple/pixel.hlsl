struct PSInput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState smp : register(s0);

float4 main(PSInput pin) : SV_TARGET
{
	return float4(0.9f, 0.2f, 0.2f, 1.0f) + tex.Sample(smp, pin.UV);
}