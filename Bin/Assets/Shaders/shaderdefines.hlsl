
struct TLight
{
	float3 pos;
	float pad;

	float3 dir;
	float pad2;

	float4 ambient;
	float4 diffuse;
	float4 spec;

	float3 att;
	float pad3;

	float spotPower;
	float range;
	int iLightType;
	float pad4;
};
struct TLightSurface
{
	float3 pos;
	float3 normal;
	float4 diffuse;
	float4 spec;
};

//Function declarations
float4 CalculateGeneralLight(TLight _light, float3 _worldPos, float3 _direction, float3 _normal, float3 _cameraPos);
float3 ParallelLight(TLightSurface v, TLight L, float3 eyePos);
float3 PointLight(TLightSurface v, TLight L, float3 eyePos);
float3 Spotlight(TLightSurface v, TLight L, float3 eyePos);

float4 ToScreenSpace(float4 _vector, matrix _world, matrix _view, matrix _projection);

//Function definitions 
float4 CalculateGeneralLight(TLight _light, float3 _worldPos, float3 _direction, float3 _normal, float3 _cameraPos)
{
	float3 LightDirection = -_direction;
	float3 Normal = normalize(_light.pos - _worldPos);
	Normal = normalize(Normal);

	float fDiffuseFactor = dot(Normal, LightDirection);

	float4 DiffuseColour = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 SpecularColour = float4(0.0f, 0.0f, 0.0f, 0.0f);

	if (fDiffuseFactor > 0.0f)
	{
		DiffuseColour = _light.diffuse * fDiffuseFactor;

		float3 VertexToCamera = normalize(_cameraPos - _worldPos);
			float3 LightReflection = normalize(reflect(_direction, Normal));
			float fSpecularFactor = dot(VertexToCamera, LightReflection);
		fSpecularFactor = pow(fSpecularFactor, _light.spotPower);
		if (fSpecularFactor > 0.0f)
		{
			SpecularColour = _light.spec * fSpecularFactor;
		}
	}
	float4 finalColour = DiffuseColour + SpecularColour;
		finalColour.a = 1.0f;
	return (DiffuseColour + SpecularColour);
}
float3 ParallelLight(TLightSurface v, TLight L, float3 eyePos)
{
	float3 litColor = float3(0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
	float3 lightVec = -L.dir;

	// Add the ambient term.
	litColor += v.diffuse.xyz * L.ambient.xyz;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

	float diffuseFactor = dot(lightVec, v.normal);
	[branch]
	if (diffuseFactor > 0.0f)
	{
		float specPower = max(v.spec.a, 1.0f);
		float3 toEye = normalize(eyePos - v.pos);
			float3 R = reflect(-lightVec, v.normal);
			float specFactor = pow(max(dot(R, toEye), 0.0f), specPower);

		// diffuse and specular terms
		litColor += diffuseFactor * v.diffuse.xyz * L.diffuse.xyz;
		litColor += specFactor * v.spec.xyz * L.spec.xyz;
	}

	return litColor;
}

float3 PointLight(TLightSurface v, TLight L, float3 eyePos)
{
	float4 vecLightColour = float4(0.0f, 0.0f, 0.0f, 1.0f);
	float3 vecLightDir = v.pos - L.pos;
	float fLightDistance = length(vecLightDir);

	vecLightColour = CalculateGeneralLight(L, v.pos, vecLightDir, v.normal, eyePos);
	//L.att = float3(0.15f, 0.02f, 5.0f);
	float fAttenuation = L.att.x +
		(L.att.y * fLightDistance) +
		(L.att.z * fLightDistance * fLightDistance);
	vecLightColour /= fAttenuation;
	vecLightColour.w = 1.0f;
	return vecLightColour.xyz;
}

float3 Spotlight(TLightSurface v, TLight L, float3 eyePos)
{
	float3 litColour = PointLight(v, L, eyePos);

	// The vector from the surface to the light.
	float3 lightVec = normalize(L.pos - v.pos);

	float spot = pow(max(dot(-lightVec, L.dir), 0.0f), L.spotPower);

	// Scale color by spotlight factor.
	return litColour * spot;
}

float4 ToScreenSpace(float4 _vector, matrix _world, matrix _view, matrix _projection)
{
	float4 outPos = mul(_vector, _world);
	outPos = mul(outPos, _view);
	outPos = mul(outPos, _projection);
	return outPos;
}