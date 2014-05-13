
// Buffers
cbuffer MatrixBuffer
{
	matrix worldMat;
	matrix viewMat;
	matrix projMat;
};

//  Vertex Types
struct VS_IN
{
	float3 pos				: POSITION;
	float2 scale			: SCALE;
	float4 colour			: COLOR;
	float2 uvTopLeft		: UVTOPLEFT;
	float2 uvBottomRight	: UVBOTTOMRIGHT;
};

struct VS_OUT
{
	float4 pos				: SV_POSITION;
	float2 scale			: SCALE;
	float4 colour			: COLOR;
	float2 uvTopLeft		: UVTOPLEFT;
	float2 uvBottomRight	: UVBOTTOMRIGHT;
};

//=============================
//	Object Shader Vertex
//=============================
VS_OUT FontVS(VS_IN _input)
{
	VS_OUT outVert;

	outVert.pos = mul(float4(_input.pos, 1.0f), worldMat);

	outVert.scale = _input.scale;
	outVert.colour = _input.colour;
	outVert.uvTopLeft = _input.uvTopLeft;
	outVert.uvBottomRight = _input.uvBottomRight;

	return outVert;
}
