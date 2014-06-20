__kernel void ProcessGrass(	__global float4* _pGrassPositions, 
							__global float4* _pGrassDirections,
							__global float4* _pObjectPositions,

							__global float4* _pOutDirections)
{
	unsigned int iWidth = get_local_id(0);
	unsigned int iHeight = get_global_id(1);

	unsigned int iIndex = (iHeight * get_local_size(0)) + iWidth;
	unsigned int iObstacle = get_group_id(0);
	_pOutDirections[iIndex] = _pGrassDirections[iIndex];

	float fDeltaTime = _pGrassPositions[0].w;
	float fGrassSpeed = 5.0f;
	
	float4 vecToObstacle = _pObjectPositions[iObstacle] - _pGrassPositions[iIndex];
	vecToObstacle.w = 0.0f;

	float fDistanceToObstacle = length(vecToObstacle);
	float fObjectRadius = _pObjectPositions[iObstacle].w;
	if(fDistanceToObstacle < fObjectRadius)
	{
		float4 offsetVector = vecToObstacle * (fObjectRadius - fDistanceToObstacle);
		_pOutDirections[iIndex] += _pOutDirections[iIndex] + ((_pOutDirections[iIndex] - offsetVector) * (fGrassSpeed * fDeltaTime));
	}

	_pOutDirections[iIndex].w = 0.0f;
	_pOutDirections[iIndex] = normalize(_pOutDirections[iIndex]);
}