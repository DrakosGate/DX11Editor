
#include "shaderdefines.hlsl"

// Buffers
cbuffer MatrixBuffer
{
	matrix worldMat;
	matrix viewMat;
	matrix projMat;
}; 

struct GS_DATA
{
	float4 finalPos : SV_POSITION;
	float3 worldPos : POSITION;
	float4 colour	: COLOR;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
	float2 texC		: TEXCOORD;
};

[maxvertexcount(6)]
void GrassGS(point GS_DATA input[1], inout TriangleStream<GS_DATA> gsOut)
{
	float3 vecGrassScale = float3(0.5f, 0.5f, 0.5f);
	float3 vertPos;

	//==================================================================================
	//	FRONT PLANE
	//==================================================================================
	//Bottom Left
	GS_DATA outVert = input[0];
	vertPos = input[0].worldPos + float3(0.0f, 0.0f, 0.0f);
	outVert.finalPos = ToScreenSpace(float4(vertPos, 1.0f), worldMat, viewMat, projMat);
	outVert.texC = float2(0.0f, 1.0f);
	gsOut.Append(outVert);
	//Top Left
	outVert = input[0];
	vertPos = input[0].worldPos + float3(0.0f, vecGrassScale.y, 0.0f);
	outVert.finalPos = ToScreenSpace(float4(vertPos, 1.0f), worldMat, viewMat, projMat);
	outVert.texC = float2(0.0f, 0.0f);
	gsOut.Append(outVert);
	//Top Right
	outVert = input[0];
	vertPos = input[0].worldPos + float3(vecGrassScale.x, vecGrassScale.y, 0.0f);
	outVert.finalPos = ToScreenSpace(float4(vertPos, 1.0f), worldMat, viewMat, projMat);
	outVert.texC = float2(1.0f, 0.0f);
	gsOut.Append(outVert);
	gsOut.RestartStrip();

	//Top Right
	outVert = input[0];
	vertPos = input[0].worldPos + float3(vecGrassScale.x, vecGrassScale.y, 0.0f);
	outVert.finalPos = ToScreenSpace(float4(vertPos, 1.0f), worldMat, viewMat, projMat);
	outVert.texC = float2(1.0f, 0.0f);
	gsOut.Append(outVert);

	//Bottom Right
	outVert = input[0];
	vertPos = input[0].worldPos + float3(vecGrassScale.x, 0.0f, 0.0f);
	outVert.finalPos = ToScreenSpace(float4(vertPos, 1.0f), worldMat, viewMat, projMat);
	outVert.texC = float2(1.0f, 1.0f);
	gsOut.Append(outVert);

	//Bottom Left
	outVert = input[0];
	vertPos = input[0].worldPos + float3(0.0f, 0.0f, 0.0f);
	outVert.finalPos = ToScreenSpace(float4(vertPos, 1.0f), worldMat, viewMat, projMat);
	outVert.texC = float2(0.0f, 1.0f);
	gsOut.Append(outVert);
	gsOut.RestartStrip();

	//==================================================================================
	//	SIDE PLANE
	//==================================================================================

}