
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

[maxvertexcount(12)]
void GrassGS(point GS_IN input[1], inout TriangleStream<GS_OUT> gsOut)
{
	GS_OUT gTri;
	float4 Origin = input[0].pos;
	float4 fGrassScale = float4(0.2f, 0.5f, 1.0f, 1.0f);
	float4 fCurrentPos = Origin;
	float fScale = 0.01f;
	fCurrentPos = Origin;
	float3 vecNormal;
	float2 fGrassDim = float2(1.0f, 0.2f);

	float4 vecOffset = float4(0.0f, 0.0f, 0.0f, 0.0f) * 0.0f;

	//Top R
	gTri.pos = fCurrentPos + ((float4(input[0].normal, 0.0f) + float4(fGrassDim.x, 0.0f, 0.0f, 0.0f)) * fGrassScale) + vecOffset;
	gTri.pos = mul(gTri.pos, gWVP);
	vecNormal = (gTri.pos - Origin);
	gTri.normal = vecNormal;
	gTri.texC = float2(1.0f, 0.0f);
	gsOut.Append(gTri);

	//Top L
	gTri = input[0];
	gTri.pos = fCurrentPos + ((float4(input[0].normal, 0.0f) + float4(-fGrassDim.x, 0.0f, 0.0f, 0.0f)) * fGrassScale) + vecOffset;
	gTri.pos = mul(gTri.pos, gWVP);
	vecNormal = (gTri.pos - Origin);
	gTri.normal = vecNormal;
	gTri.texC = float2(0.0f, 0.0f);
	gsOut.Append(gTri);

	//Bottom L
	gTri = input[0];
	gTri.pos = fCurrentPos + (float4(-fGrassDim.x, 0.0f, 0.0f, 0.0f) * fGrassScale);
	gTri.pos = mul(gTri.pos, gWVP);
	gTri.normal = vecNormal;
	gTri.texC = float2(0.0f, 1.0f);
	gsOut.Append(gTri);

	//Top R
	gTri = input[0];
	gTri.pos = fCurrentPos + ((float4(input[0].normal, 0.0f) + float4(fGrassDim.x, 0.0f, 0.0f, 0.0f)) * fGrassScale) + vecOffset;
	gTri.pos = mul(gTri.pos, gWVP);
	vecNormal = (gTri.pos - Origin);
	gTri.normal = vecNormal;
	gTri.texC = float2(1.0f, 0.0f);
	gsOut.Append(gTri);

	//Bottom Left
	gTri = input[0];
	gTri.pos = fCurrentPos + (float4(-fGrassDim.x, 0.0f, 0.0f, 0.0f) * fGrassScale);
	gTri.pos = mul(gTri.pos, gWVP);
	gTri.normal = vecNormal;
	gTri.texC = float2(0.0f, 1.0f);
	gsOut.Append(gTri);

	//Bottom Right
	gTri = input[0];
	gTri.pos = fCurrentPos + (float4(fGrassDim.x, 0.0f, 0.0f, 0.0f) * fGrassScale);
	gTri.pos = mul(gTri.pos, gWVP);
	gTri.normal = vecNormal;
	gTri.texC = float2(1.0f, 1.0f);
	gsOut.Append(gTri);
	gsOut.RestartStrip();

	//============================================================================
	//	Sideways grass
	//============================================================================

	//Top R
	gTri.pos = fCurrentPos + ((float4(input[0].normal, 0.0f) + float4(0.0f, 0.0f, fGrassDim.y, 0.0f)) * fGrassScale) + vecOffset;
	gTri.pos = mul(gTri.pos, gWVP);
	vecNormal = (gTri.pos - Origin);
	gTri.normal = vecNormal;
	gTri.texC = float2(1.0f, 0.0f);
	gsOut.Append(gTri);

	//Top L
	gTri = input[0];
	gTri.pos = fCurrentPos + ((float4(input[0].normal, 0.0f) + float4(0.0f, 0.0f, -fGrassDim.y, 0.0f)) * fGrassScale) + vecOffset;
	gTri.pos = mul(gTri.pos, gWVP);
	vecNormal = (gTri.pos - Origin);
	gTri.normal = vecNormal;
	gTri.texC = float2(0.0f, 0.0f);
	gsOut.Append(gTri);

	//Bottom L
	gTri = input[0];
	gTri.pos = fCurrentPos + (float4(0.0f, 0.0f, -fGrassDim.y, 0.0f) * fGrassScale);
	gTri.pos = mul(gTri.pos, gWVP);
	gTri.normal = vecNormal;
	gTri.texC = float2(0.0f, 1.0f);
	gsOut.Append(gTri);

	//Top R
	gTri = input[0];
	gTri.pos = fCurrentPos + ((float4(input[0].normal, 0.0f) + float4(0.0f, 0.0f, fGrassDim.y, 0.0f)) * fGrassScale) + vecOffset;
	gTri.pos = mul(gTri.pos, gWVP);
	vecNormal = (gTri.pos - Origin);
	gTri.normal = vecNormal;
	gTri.texC = float2(1.0f, 0.0f);
	gsOut.Append(gTri);

	//Bottom Left
	gTri = input[0];
	gTri.pos = fCurrentPos + (float4(0.0f, 0.0f, -fGrassDim.y, 0.0f) * fGrassScale);
	gTri.pos = mul(gTri.pos, gWVP);
	gTri.normal = vecNormal;
	gTri.texC = float2(0.0f, 1.0f);
	gsOut.Append(gTri);

	//Bottom Right
	gTri = input[0];
	gTri.pos = fCurrentPos + (float4(0.0f, 0.0f, fGrassDim.y, 0.0f) * fGrassScale);
	gTri.pos = mul(gTri.pos, gWVP);
	gTri.normal = vecNormal;
	gTri.texC = float2(1.0f, 1.0f);
	gsOut.Append(gTri);
	gsOut.RestartStrip();
}
MRTPSOut GrassMRTPS(VS_OUT pIn) : SV_Target
{
	MRTPSOut pOut;
	// Get materials from texture maps.
	float4 diffuse = gDiffuseMap.Sample(gTriLinearSam, pIn.texC);
		float fAlpha = diffuse.a;

	//pOut.oDiffuse = float4(pIn.posW, fAlpha);
	pOut.oDiffuse = diffuse;
	pOut.oNormal = float4(normalize(pIn.normal), fAlpha);
	pOut.oPosition = float4(pIn.posW, fAlpha);

	return pOut;
}