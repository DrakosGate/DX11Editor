//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   grass.cpp
//  Description :   Code for class CGrass
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <d3d11.h>

// Local Includes

// This Include
#include "grass.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CGrass class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CGrass::CGrass()
{
	m_fGrassSpeed = 1.0f;
	m_fGrassStiffness = 1.0f;
}

/**
*
* CGrass class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CGrass::~CGrass()
{
	
}
/**
*
* CGrass class RecreateGrassMesh offsets grass mesh based on camera position
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CGrass::RecreateGrassMesh(ID3D11Device* _pDevice, D3DXVECTOR3& _rCameraPos, CRenderEntity** _pEntities, int _iNumEntities, float _fDeltaTime)
{
	float fHalfScale = m_fModelScale * 0.5f;
	//Delete and recreate vertex buffer data
	m_pVertexBuffer->Release();
	D3DXVECTOR3 vecNormalTarget = D3DXVECTOR3(0.0f, m_fGrassStiffness, 0.0f);

	for(int iVertex = 0; iVertex < m_iVertexCount; ++iVertex)
	{
		m_pVertices[iVertex].normal += (vecNormalTarget - m_pVertices[iVertex].normal) * m_fGrassSpeed * _fDeltaTime;
		//Width
		if(m_pVertices[iVertex].pos.x > _rCameraPos.x + fHalfScale)
		{
			m_pVertices[iVertex].pos.x -= m_fModelScale;
		}
		if(m_pVertices[iVertex].pos.x < _rCameraPos.x - fHalfScale)
		{
			m_pVertices[iVertex].pos.x += m_fModelScale;
		}
		//Height
		if(m_pVertices[iVertex].pos.z > _rCameraPos.z + fHalfScale)
		{
			m_pVertices[iVertex].pos.z -= m_fModelScale;
		}
		if(m_pVertices[iVertex].pos.z < _rCameraPos.z - fHalfScale)
		{
			m_pVertices[iVertex].pos.z += m_fModelScale;
		}

		//Avoid entities
		D3DXVECTOR3 vecToEntity;
		float fAvoidanceRange = 1.0f;
		for(int iEntity = 0; iEntity < _iNumEntities; ++iEntity)
		{
			vecToEntity = _pEntities[iEntity]->GetPosition() - m_pVertices[iVertex].pos;
			float fDistanceToEntity = D3DXVec3LengthSq(&vecToEntity);
			if(fDistanceToEntity < fAvoidanceRange)
			{
				m_pVertices[iVertex].normal -= (vecToEntity * (fAvoidanceRange - fDistanceToEntity) * 2.0f) * m_fGrassSpeed * _fDeltaTime;
			}
		}
		if(m_pVertices[iVertex].normal.y < 0.5f)
		{
			m_pVertices[iVertex].normal.y = 0.5f;
		}
		D3DXVec3Normalize(&m_pVertices[iVertex].normal, &m_pVertices[iVertex].normal);
	}

	CreateVertexBuffer(_pDevice);
}