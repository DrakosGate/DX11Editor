#include "shaderdefines.hlsl"

//Textures
Texture2D g_textureArray[20];
SamplerState textureSampler;

struct PS_IN
{
	float4 pos		: SV_POSITION;
	float3 dir		: DIRECTION;
	float2 scale	: SCALE;
	float4 colour	: COLOR;
	float rotation : ROTATION;
	float2 texC		:TEXCOORD;
	int textureID : TEXID;
};
struct MRTPS_OUT
{
	float4 oDiffuse		: Color0;
	float4 oNormal		: Color1;
	float4 oPosition	: Color2;
};

//=============================
//	Object Shader Pixel
//=============================
float4 LinePS(PS_IN _input) : SV_TARGET
{
	float4 diffuseColour = _input.colour;
	return diffuseColour;
}
//=============================
//	Object Shader Pixel
//=============================
MRTPS_OUT MRTLinePS(PS_IN _input) : SV_TARGET
{
	MRTPS_OUT pOut;

	pOut.oDiffuse = _input.colour;
	pOut.oNormal = float4(float3(0.0f, 1.0f, 0.0f), _input.colour.a);
	pOut.oPosition = _input.pos;

	return pOut;
}