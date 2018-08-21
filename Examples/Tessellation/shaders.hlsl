/* vertex shader */
struct VertexIn
{
	float3 Position : POSITION;
};

struct VertexOut
{
	float3 Position : POSITION;
};

VertexOut mainVS(VertexIn vin)
{ 
	VertexOut vout;
	vout.Position = vin.Position;
	return vout;
} 


cbuffer cbData : register(b0)
{
	matrix matWVP;
	float3 cameraPosition;
}


/* constant hull shader */
struct PatchTessFactor
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

static const float minDistance = 5.0f;
static const float maxDistance = 48.0f;

PatchTessFactor mainCHS(InputPatch<VertexOut, 4> patch)
{
	PatchTessFactor factor;
	
	float3 center = 0.25f*(patch[0].Position + patch[1].Position + patch[2].Position + patch[3].Position);
	float dist = distance(center, cameraPosition);
	
	float tf = 1.0f + 63.0f*saturate((maxDistance- dist)/(maxDistance -minDistance));

	factor.EdgeTess[0] = tf;
	factor.EdgeTess[1] = tf;
	factor.EdgeTess[2] = tf;
	factor.EdgeTess[3] = tf;
	
	factor.InsideTess[0] = tf;
	factor.InsideTess[1] = tf;

	return factor;
}


/* control point hull shader */
struct HullOut
{
	float3 Position : POSITION;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("mainCHS")]
[maxtessfactor(64.0f)]
HullOut mainHS(InputPatch<VertexOut, 4> p, uint i : SV_OutputControlPointID)
{
	HullOut hout;
	hout.Position = p[i].Position;
	return hout;
}


/* domain shader */
struct DomainOut
{
	float4 Position : SV_POSITION;
};

[domain("quad")]
DomainOut mainDS(PatchTessFactor patchTess, float2 uv : SV_DomainLocation, const OutputPatch<HullOut, 4> quad)
{ 
	DomainOut dout;

	// bilinear interpolation
	float3 v1 = lerp(quad[0].Position, quad[1].Position, uv.x);
	float3 v2 = lerp(quad[2].Position, quad[3].Position, uv.x);
	float3 p = lerp(v1, v2, uv.y);

	// displacement
	p.y = sin(p.x) + cos(p.z);

	dout.Position = mul(float4(p, 1.0f), matWVP);

	return dout;
}


/* pixel shader */
float4 mainPS(DomainOut pin) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}