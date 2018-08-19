cbuffer cbPerFrame : register(b0)
{
	matrix matVP;
	matrix matWorld;
	float3 viewPosition;
};

struct GSInput
{
	float3 Position : POSITION;
};

struct GSOutput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

// the size/2 of the tree billboard
static const float2 halfSize = float2(0.035f, 0.035f);

// the up vector in our world
static const float3 viewUp = float3(0, 1, 0);

[maxvertexcount(4)]
void main(
	point GSInput input[1],
	inout TriangleStream< GSOutput > output
)
{
	matrix wvp = mul(matWorld, matVP);

	float3 pos = input[0].Position;

	float3 look = normalize(viewPosition - pos);
	float3 viewRight = normalize(cross(viewUp, look));
	
	GSOutput el;

	el.Position = float4(pos - halfSize.x*viewRight - halfSize.y*viewUp, 1);
	el.Position = mul(el.Position, wvp);
	el.UV = float2(0, 1);
	output.Append(el);

	el.Position = float4(pos - halfSize.x*viewRight + halfSize.y*viewUp, 1);
	el.Position = mul(el.Position, wvp);
	el.UV = float2(0,0);
	output.Append(el);

	el.Position = float4(pos + halfSize.x*viewRight - halfSize.y*viewUp, 1);
	el.Position = mul(el.Position, wvp);
	el.UV = float2(1,1);
	output.Append(el);

	el.Position = float4(pos + halfSize.x*viewRight + halfSize.y*viewUp, 1);
	el.Position = mul(el.Position, wvp);
	el.UV = float2(1,0);
	output.Append(el);
}