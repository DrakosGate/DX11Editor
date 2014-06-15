#include "shaderdefines.hlsl"

//Textures
Texture2D diffuseMap;
Texture2D depthMap;
SamplerState textureSampler;

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

//=============================
//	Object Shader Pixel
//=============================
float4 FinalColourPS(PS_IN _input) : SV_TARGET
{
	float4 diffuseColour = diffuseMap.Sample(textureSampler, _input.texC);
	return diffuseColour;
}
float4 PixelatePS(PS_IN _input) : SV_TARGET
{
	float fPixelSize = 0.008f;
	float2 pixelTexCoord = float2(_input.texC.x - fmod(_input.texC.x, fPixelSize), _input.texC.y - fmod(_input.texC.y, fPixelSize));
	float4 diffuseColour = diffuseMap.Sample(textureSampler, pixelTexCoord);
	diffuseColour.a = 1.0f;
	return diffuseColour;
}
float4 RadialBlurPS(PS_IN _input) : SV_Target
{
	// Get materials from texture maps.
	float4 diffuse = diffuseMap.Sample(textureSampler, _input.texC);
	float fDepth = depthMap.Sample(textureSampler, _input.texC).r;
	int iNumSamples = 5;
	//Calculate distance of pixel from centre of screen
	float fRatio = length(float2(0.5f, 0.5f) - _input.texC);
	float fBlurDistance = (0.01f / iNumSamples) * (fRatio * 0.5f);
	int iSampleCount = 0;
	float fBlurPower = 1.0f;
	//Blur texture samples
	float4 blur = float4(0.0f, 0.0f, 0.0f, 0.0f);
	for (int iSample = 0; iSample < iNumSamples; ++iSample)
	{
		blur += diffuseMap.Sample(textureSampler, _input.texC + float2(iSample * fBlurDistance, 0.0f)) * fBlurPower;
		blur += diffuseMap.Sample(textureSampler, _input.texC + float2(-iSample * fBlurDistance, 0.0f)) * fBlurPower;
		blur += diffuseMap.Sample(textureSampler, _input.texC + float2(0.0f, iSample * fBlurDistance)) * fBlurPower;
		blur += diffuseMap.Sample(textureSampler, _input.texC + float2(0.0f, -iSample * fBlurDistance)) * fBlurPower;
								  
		blur += diffuseMap.Sample(textureSampler, _input.texC + float2(iSample * fBlurDistance, iSample * fBlurDistance)) * fBlurPower;
		blur += diffuseMap.Sample(textureSampler, _input.texC + float2(-iSample * fBlurDistance, iSample * fBlurDistance)) * fBlurPower;
		blur += diffuseMap.Sample(textureSampler, _input.texC + float2(iSample * fBlurDistance, -iSample * fBlurDistance)) * fBlurPower;
		blur += diffuseMap.Sample(textureSampler, _input.texC + float2(-iSample * fBlurDistance, -iSample * fBlurDistance)) * fBlurPower;
		iSampleCount += 8;
	}
	diffuse = (blur / iSampleCount);
	//diffuse.rgb = fDepth;
	diffuse.a = 1.0f;

	return diffuse;
}