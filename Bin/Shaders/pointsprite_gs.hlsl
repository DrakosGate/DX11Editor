
// Buffers
cbuffer MatrixBuffer
{
	matrix worldMat;
	matrix viewMat;
	matrix projMat;
}; 

struct GS_IN
{
	float4 pos		: SV_POSITION;
	float3 dir		: DIRECTION;
	float2 scale	: SCALE;
	float4 colour	: COLOR;
	float rotation : ROTATION;
	int textureID : TEXID;
};
struct GS_OUT
{
	float4 pos		: SV_POSITION;
	float3 dir		: DIRECTION;
	float2 scale	: SCALE;
	float4 colour	: COLOR;
	float rotation : ROTATION;
	float2 texC		:TEXCOORD;
	int textureID : TEXID;
};

[maxvertexcount(6)]
void PointGS(point GS_IN input[1], inout TriangleStream<GS_OUT> OutputStream)
{
	GS_OUT gOut[6];

	float4 topRight = float4(input[0].scale.x, 0.0f, 0.0f, 0.0f);
	float4 bottomLeft = float4(0.0f, -input[0].scale.y, 0.0f, 0.0f);
	float4 bottomRight = float4(input[0].scale.x, -input[0].scale.y, 0.0f, 0.0f);
	float2 uv[4] = 
	{
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f),
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
	};

	matrix matTransform = projMat * viewMat * worldMat;
	
	gOut[0].pos = mul(input[0].pos, matTransform);
	gOut[1].pos = mul(input[0].pos + topRight, matTransform);
	gOut[2].pos = mul(input[0].pos + bottomLeft, matTransform);

	gOut[3].pos = mul(input[0].pos + bottomLeft, matTransform);
	gOut[4].pos = mul(input[0].pos + bottomRight, matTransform);
	gOut[5].pos = mul(input[0].pos + topRight, matTransform);

	gOut[0].texC = uv[0];
	gOut[1].texC = uv[1];
	gOut[2].texC = uv[2];
	gOut[3].texC = uv[2];
	gOut[4].texC = uv[3];
	gOut[5].texC = uv[1];
	
	for (int i = 0; i < 6; i++)
	{
		gOut[i].dir = input[0].dir;
		gOut[i].scale = input[0].scale;
		gOut[i].colour = input[0].colour;
		gOut[i].rotation = input[0].rotation;
		gOut[i].textureID = input[0].textureID;
		OutputStream.Append(gOut[i]);
	}
}