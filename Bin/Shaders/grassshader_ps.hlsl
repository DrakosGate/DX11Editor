#include "shaderdefines.hlsl"

//Textures
Texture2D diffuseMap;
Texture2D normalMap;
SamplerState textureSampler;

struct PS_IN
{
	float4 finalPos : SV_POSITION;
	float3 worldPos : POSITION;
	float4 colour	: COLOR;
	float3 normal	: NORMAL;
	float3 grassNormal: GRASSNORMAL;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
	float2 texC		: TEXCOORD;
};
struct MRTPS_OUT
{
	float4 oDiffuse		: Color0;
	float4 oNormal		: Color1;
	float4 oPosition	: Color2;
	float4 oDepth		: Color3;
};

//=============================
//	Object Shader Pixel
//=============================
MRTPS_OUT GrassMRTPS(PS_IN _input) : SV_TARGET
{
	MRTPS_OUT pOut;

	float4 diffuseColour = diffuseMap.Sample(textureSampler, _input.texC);
	float fAlpha = diffuseColour.a;

	pOut.oDiffuse = diffuseColour;
	pOut.oNormal = float4(_input.normal, fAlpha);
	pOut.oPosition = float4(_input.worldPos, 1.0f);

	float fDepth = pOut.oPosition.z / pOut.oPosition.w;
	pOut.oDepth = float4(fDepth, fDepth, fDepth, 1.0f);

	return pOut;
}