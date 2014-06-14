__kernel void ProcessAI(__global float4* _pPositions, 
						__global float4* _pDirection, 
						__global float4* _pWaypoints,
						__global float4* _pAIData,

						__global float4* _pOutPositions,
						__global float4* _pOutDirections)
{
	/*
	int iStartingPosition = _iSection * m_iDivisionSize;
	for (int iBlade = 0; iBlade < m_iDivisionSize; ++iBlade)
	{
		int iCurrentVertex = iStartingPosition + iBlade;
		if (iCurrentVertex < m_iVertexCount)
		{
			//Avoid entities
			D3DXVECTOR3 vecToEntity;
			float fAvoidanceRange = 1.0f;
			unsigned int iNumEntities = m_pCollisionObjects->size();
			for (unsigned int iEntity = 0; iEntity < iNumEntities; ++iEntity)
			{
				fAvoidanceRange = ((*m_pCollisionObjects)[iEntity]->GetRadius() * (*m_pCollisionObjects)[iEntity]->GetRadius()) * 0.75f;

				vecToEntity = (*m_pCollisionObjects)[iEntity]->GetPosition() - m_pVertices[iCurrentVertex].pos;
				float fDistanceToEntity = D3DXVec3LengthSq(&vecToEntity);
				if (fDistanceToEntity < fAvoidanceRange)
				{
					m_pVertices[iCurrentVertex].normal -= (vecToEntity * (fAvoidanceRange - fDistanceToEntity)) * m_fGrassSpeed * _fDeltaTime;
				}
			}
			if (m_pVertices[iCurrentVertex].normal.y < 0.5f)
			{
				m_pVertices[iCurrentVertex].normal.y = 0.5f;
			}
			D3DXVec3Normalize(&m_pVertices[iCurrentVertex].normal, &m_pVertices[iCurrentVertex].normal);
		}
	}
	*/
}