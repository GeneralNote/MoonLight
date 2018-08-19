struct VSInput
{
	float3 Position : POSITION;
};

struct VSOutput
{
	float3 Position : POSITION;
};

VSOutput main(VSInput vin)
{
	VSOutput vout = (VSOutput)0;
	vout.Position = float4(vin.Position, 1);
	return vout;
}