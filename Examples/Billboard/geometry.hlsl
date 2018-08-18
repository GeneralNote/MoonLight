cbuffer cbPerFrame : register(b0)
{
	matrix matVP;
	matrix matWorld;
	float ApplyVP;
};

cbuffer cbCamera : register(b1)
{
	float3 viewPosition;
};

struct GSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD;
};

struct GSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float2 UV : TEXCOORD;
};

// the size/2 of the tree billboard
static const float2 halfSize = float2(3.0f,3.0f);

// blender rotated everything so we have a slightly different viewUp vector
static const float3 viewUp = float3(1, 0, 0);

[maxvertexcount(4)]
void main(
	point GSInput input[1],
	inout TriangleStream< GSOutput > output
)
{
	float4 pos = input[0].Position;

	float3 look = normalize(float3(0, viewPosition.yz - pos.yz));
	float3 viewRight = normalize(cross(viewUp, look));
	
	GSOutput el;
	el.Color = input[0].Color;

	el.Position = float4(pos.xyz - halfSize.x*viewRight - halfSize.y*viewUp, 1);
	el.Position = mul(el.Position, matVP);
	el.UV = float2(0, 1);
	output.Append(el);

	el.Position = float4(pos.xyz - halfSize.x*viewRight + halfSize.y*viewUp, 1);
	el.Position = mul(el.Position, matVP);
	el.UV = float2(0,0);
	output.Append(el);

	el.Position = float4(pos.xyz + halfSize.x*viewRight - halfSize.y*viewUp, 1);
	el.Position = mul(el.Position, matVP);
	el.UV = float2(1,1);
	output.Append(el);

	el.Position = float4(pos.xyz + halfSize.x*viewRight + halfSize.y*viewUp, 1);
	el.Position = mul(el.Position, matVP);
	el.UV = float2(1,0);
	output.Append(el);
}