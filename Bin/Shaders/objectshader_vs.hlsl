
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
	float3 pos		: POSITION;
	float4 colour	: COLOR;
	float3 normal	: NORMAL;
	float3 tangent	: TANGENT;
	float3 binormal	: BINORMAL;
	float2 texC		: TEXCOORD;
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

//=============================
//	Object Shader Vertex
//=============================
PS_IN ObjectVS(VS_IN _input)
{
	PS_IN outVert;

	//Calculate vertex position 
	outVert.worldPos = mul(float4(_input.pos, 1.0f), worldMat).xyz;
	outVert.finalPos = mul(float4(outVert.worldPos, 1.0f), viewMat);
	outVert.finalPos = mul(outVert.finalPos, projMat);

	//Vertex colour
	outVert.colour = _input.colour;
	outVert.normal = mul(float4(_input.normal, 1.0f), worldMat).xyz;
	outVert.tangent = mul(float4(_input.tangent, 1.0f), worldMat).xyz;
	outVert.binormal = mul(float4(_input.binormal, 1.0f), worldMat).xyz;

	outVert.normal = normalize(outVert.normal);
	outVert.tangent = normalize(outVert.tangent);
	outVert.binormal = normalize(outVert.binormal);

	outVert.texC = _input.texC;

	return outVert;
}
