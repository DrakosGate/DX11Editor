#include "shaderdefines.hlsl"

//Textures
Texture2D g_textureArray[30];
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
float4 SampleTextureArray(PS_IN _input)
{
	float4 diffuseColour;
	switch (_input.textureID)
	{
		case  0: diffuseColour = g_textureArray[0].Sample(textureSampler, _input.texC);
			break;
		case  1: diffuseColour = g_textureArray[1].Sample(textureSampler, _input.texC);
			break;
		case  2: diffuseColour = g_textureArray[2].Sample(textureSampler, _input.texC);
			break;
		case  3: diffuseColour = g_textureArray[3].Sample(textureSampler, _input.texC);
			break;
		case  4: diffuseColour = g_textureArray[4].Sample(textureSampler, _input.texC);
			break;
		case  5: diffuseColour = g_textureArray[5].Sample(textureSampler, _input.texC);
			break;
		case  6: diffuseColour = g_textureArray[6].Sample(textureSampler, _input.texC);
			break;
		case  7: diffuseColour = g_textureArray[7].Sample(textureSampler, _input.texC);
			break;
		case  8: diffuseColour = g_textureArray[8].Sample(textureSampler, _input.texC);
			break;
		case  9: diffuseColour = g_textureArray[9].Sample(textureSampler, _input.texC);
			break;
		case 10: diffuseColour = g_textureArray[10].Sample(textureSampler, _input.texC);
			break;
		case 11: diffuseColour = g_textureArray[11].Sample(textureSampler, _input.texC);
			break;
		case 12: diffuseColour = g_textureArray[12].Sample(textureSampler, _input.texC);
			break;
		case 13: diffuseColour = g_textureArray[13].Sample(textureSampler, _input.texC);
			break;
		case 14: diffuseColour = g_textureArray[14].Sample(textureSampler, _input.texC);
			break;
		case 15: diffuseColour = g_textureArray[15].Sample(textureSampler, _input.texC);
			break;
		case 16: diffuseColour = g_textureArray[16].Sample(textureSampler, _input.texC);
			break;
		case 17: diffuseColour = g_textureArray[17].Sample(textureSampler, _input.texC);
			break;
		case 18: diffuseColour = g_textureArray[18].Sample(textureSampler, _input.texC);
			break;
		case 19: diffuseColour = g_textureArray[19].Sample(textureSampler, _input.texC);
			break;
		case 20: diffuseColour = g_textureArray[20].Sample(textureSampler, _input.texC);
			break;								
		case 21: diffuseColour = g_textureArray[21].Sample(textureSampler, _input.texC);
			break;								
		case 22: diffuseColour = g_textureArray[22].Sample(textureSampler, _input.texC);
			break;								
		case 23: diffuseColour = g_textureArray[23].Sample(textureSampler, _input.texC);
			break;								
		case 24: diffuseColour = g_textureArray[24].Sample(textureSampler, _input.texC);
			break;								
		case 25: diffuseColour = g_textureArray[25].Sample(textureSampler, _input.texC);
			break;								
		case 26: diffuseColour = g_textureArray[26].Sample(textureSampler, _input.texC);
			break;								
		case 27: diffuseColour = g_textureArray[27].Sample(textureSampler, _input.texC);
			break;								
		case 28: diffuseColour = g_textureArray[28].Sample(textureSampler, _input.texC);
			break;								
		case 29: diffuseColour = g_textureArray[29].Sample(textureSampler, _input.texC);
			break;
		default: diffuseColour = float4(1, 0, 1, 1);
			break;
	}
	return diffuseColour;
}
//=============================
//	Object Shader Pixel
//=============================
float4 PointPS(PS_IN _input) : SV_TARGET
{
	float4 diffuseColour = SampleTextureArray(_input);
	diffuseColour *= _input.colour;
	//diffuseColour.a = 0.5f;
	return diffuseColour;
}
