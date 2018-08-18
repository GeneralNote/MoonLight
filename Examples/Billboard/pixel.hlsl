cbuffer cbPerFrame : register(b0)
{
	matrix matVP;
	matrix matWorld;
	float ApplyVP;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD;
};

Texture2D tex : register(t0);
SamplerState smp : register(s0);

float4 main(PSInput pin) : SV_TARGET
{
	if (ApplyVP == 1.0f)
		return tex.Sample(smp, pin.UV);
	return pin.Color;
}