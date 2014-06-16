__kernel void ProcessAI(__global float4* _pPositions, 
						__global float4* _pDirection, 
						__global float4* _pWaypoints,
						__global float4* _pAIData,

						__global float4* _pOutPositions,
						__global float4* _pOutDirections)
{
	unsigned int iIndex = get_global_id(0);

	float fDeltaTime = _pPositions[0].w;
	float fMovementSpeed = _pAIData[iIndex].x;
	float fRotationSpeed = _pAIData[iIndex].y;
	float fRadius = _pAIData[iIndex].z;
	
	_pPositions[iIndex].w = 0.0f;
	_pWaypoints[iIndex].w = 0.0f;
	float4 vecToWaypoint = _pWaypoints[iIndex] - _pPositions[iIndex];
	vecToWaypoint.y = 0.0f;
	vecToWaypoint = normalize(vecToWaypoint);

	_pOutDirections[iIndex] = _pDirection[iIndex] + vecToWaypoint * fRotationSpeed * fDeltaTime;
	_pOutDirections[iIndex] = normalize(_pOutDirections[iIndex]);
	_pOutPositions[iIndex] = _pPositions[iIndex] + (_pOutDirections[iIndex] * fMovementSpeed * fDeltaTime);
}