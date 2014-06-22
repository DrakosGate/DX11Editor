#include "shaderdefines.hlsl"

//Textures
Texture2D diffuseMap;
Texture2D normalMap;
Texture2D positionMap;
Texture2D depthMap;
SamplerState textureSampler;

//Lighting
#define LIGHTCOUNT 50
cbuffer LightBuffer 
{
	TLight gLight[LIGHTCOUNT];
	
	float3 gCameraPos;
	int gActiveLightCount;
};

struct PS_IN
{
	float4 finalPos : SV_POSITION;
	float3 worldPos : POSITION;
	float4 colour	: COLOR;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
	float2 texC		: TEXCOORD;
};

//Function declarations
float4 CalculateLighting(float3 inPosition, float3 inNormal, float4 diffuse, float4 spec, int _iLightIndex);

//=============================
//	Object Shader Pixel
//=============================
float4 DeferredPS(PS_IN _input) : SV_TARGET
{
	// Get materials from texture maps.
	float4 diffuse = diffuseMap.Sample(textureSampler, _input.texC);
	float3 normal = normalMap.Sample(textureSampler, _input.texC).xyz;
	float3 position = positionMap.Sample(textureSampler, _input.texC).xyz;
	float fDepth = depthMap.Sample(textureSampler, _input.texC).r;
	float fAlpha = diffuse.a;

	// Map [0,1] --> [0,256]
	float4 fLightColour = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int iLight = 0; iLight < gActiveLightCount; ++iLight)
	{
		fLightColour += CalculateLighting(position, normalize(normal), diffuse, float4(1.0f, 0.0f, 0.0f, 1.0f), iLight);
	}
	diffuse *= fLightColour;
	diffuse.a = 1.0f;//fAlpha;

	return diffuse;
}
//Lighting function
float4 CalculateLighting(float3 inPosition, float3 inNormal, float4 diffuse, float4 spec, int _iLightIndex)
{
	// Interpolating normal can make it not be of unit length so normalize it.
	float3 normalW = normalize(inNormal);

	// Compute the lit color for this pixel.
	TLightSurface surfaceInformation = { inPosition, normalW, diffuse, spec };

	//Calculate light according to light type
	float3 litColor;
	if (gLight[_iLightIndex].iLightType == 0)
	{
		litColor = ParallelLight(surfaceInformation, gLight[_iLightIndex], gCameraPos);
	}
	else if (gLight[_iLightIndex].iLightType == 1)
	{
		litColor = PointLight(surfaceInformation, gLight[_iLightIndex], gCameraPos);
	}
	else
	{
		litColor = Spotlight(surfaceInformation, gLight[_iLightIndex], gCameraPos);
	}

	return float4(litColor, 1.0f);
}