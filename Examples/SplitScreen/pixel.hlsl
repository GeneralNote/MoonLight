struct PSInput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

float4 main(PSInput pin) : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}