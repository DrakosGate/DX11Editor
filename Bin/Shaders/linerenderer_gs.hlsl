
#include "shaderdefines.hlsl"

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

[maxvertexcount(2)]
void LineGS(point GS_IN input[1], inout LineStream<GS_OUT> OutputStream)
{
	GS_OUT gOut[2];

	float4 startPos = input[0].pos;
	float4 targetPos = input[0].pos + float4(input[0].dir, 0.0f);

	float2 uv[2] = 
	{
		float2(0.0f, 0.0f),
		float2(1.0f, 1.0f)
	};

	matrix matTransform = worldMat * viewMat * projMat;
	
	gOut[0].pos = ToScreenSpace(startPos, worldMat, viewMat, projMat);
	gOut[1].pos = ToScreenSpace(targetPos, worldMat, viewMat, projMat);

	gOut[0].texC = uv[0];
	gOut[1].texC = uv[1];
	
	for (int i = 0; i < 2; i++)
	{
		gOut[i].dir = input[0].dir;
		gOut[i].scale = input[0].scale;
		gOut[i].colour = input[0].colour;
		gOut[i].rotation = input[0].rotation;
		gOut[i].textureID = input[0].textureID;
		OutputStream.Append(gOut[i]);
	}
}