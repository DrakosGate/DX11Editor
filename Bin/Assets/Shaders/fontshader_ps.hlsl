#include "shaderdefines.hlsl"

//Textures
Texture2D diffuseMap;
SamplerState textureSampler;

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float2 scale	: SCALE;
	float4 colour	: COLOR;
	float2 texC		:TEXCOORD;
};

//=============================
//	Object Shader Pixel
//=============================
float4 FontPS(PS_IN _input) : SV_TARGET
{
	float4 diffuse = diffuseMap.Sample(textureSampler, _input.texC);
	diffuse *= _input.colour;
	return diffuse;
}
