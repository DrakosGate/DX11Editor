//
//  File Name   :   grass.cpp
//  Description :   Code for class CGrass
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <d3d11.h>

// Local Includes
#include "resourcemanager.h"

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
: m_pCollisionObjects(0)
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
* CGrass class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 device
* @param _pResourceManager Resource manager
* @return Returns true
*
*/
bool
CGrass::Initialise(ID3D11Device* _pDevice, CResourceManager* _pResourceManager, int _iTerrainWidth, int _iTerrainHeight, float _fScale, D3DXVECTOR2& _rVecTiling, D3DXCOLOR& _rColour, int _iProcessingDivisionSize)
{
	LoadTerrain(_pDevice, _iTerrainWidth, _iTerrainHeight, _fScale, _rVecTiling, _rColour);
	m_iDivisionSize = m_iVertexCount / _iProcessingDivisionSize;

	//Create texture for grass normal data
	int iCurrentPixel = 0;
	std::string sTextureName = "grassnormals";
	TUCHARColour* pTerrainTexture = new TUCHARColour[_iTerrainWidth * _iTerrainHeight];
	for (int iHeight = 0; iHeight < _iTerrainHeight; ++iHeight)
	{
		for (int iWidth = 0; iWidth < _iTerrainWidth; ++iWidth)
		{
			pTerrainTexture[iCurrentPixel].r = static_cast<unsigned char>(m_pVertices[iCurrentPixel].normal.x * 255.0f);
			pTerrainTexture[iCurrentPixel].g = static_cast<unsigned char>(m_pVertices[iCurrentPixel].normal.z * 255.0f);
			pTerrainTexture[iCurrentPixel].b = static_cast<unsigned char>(m_pVertices[iCurrentPixel].normal.y * 255.0f);
			pTerrainTexture[iCurrentPixel].a = 255;
			++iCurrentPixel;
		}
	}
	//SetNormalMap(_pResourceManager->CreateTextureFromData(_pDevice, reinterpret_cast<unsigned char*>(pTerrainTexture), sTextureName, _iTerrainWidth, _iTerrainHeight));
	delete[] pTerrainTexture;

	m_fGrassSpeed = 5.0f;
	m_fGrassStiffness = 2.0f;

	return true;
}
void 
CGrass::SendCollisionData(std::vector<CRenderEntity*>* _pCollisionObjects)
{
	m_pCollisionObjects = _pCollisionObjects;
}
void
CGrass::ProcessGrassSection(int _iSection, float _fDeltaTime)
{
	if (m_pCollisionObjects)
	{
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
	}
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
CGrass::RecreateGrassMesh(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, D3DXVECTOR3& _rCameraPos, std::vector<CRenderEntity*>& _pEntities, float _fDeltaTime)
{
	float fHalfScale = m_fModelScale * 0.5f;
	//Delete and recreate vertex buffer data
	m_pVertexBuffer->Release();
	D3DXVECTOR3 vecNormalTarget = D3DXVECTOR3(0.0f, m_fGrassStiffness, 0.0f);
	
	for (int iVertex = 0; iVertex < m_iVertexCount; ++iVertex)
	{
		m_pVertices[iVertex].normal += (vecNormalTarget - m_pVertices[iVertex].normal) * m_fGrassStiffness * _fDeltaTime;
		//Width
		if (m_pVertices[iVertex].pos.x > _rCameraPos.x + fHalfScale)
		{
			m_pVertices[iVertex].pos.x -= m_fModelScale;
		}
		if (m_pVertices[iVertex].pos.x < _rCameraPos.x - fHalfScale)
		{
			m_pVertices[iVertex].pos.x += m_fModelScale;
		}
		//Height
		if (m_pVertices[iVertex].pos.z > _rCameraPos.z + fHalfScale)
		{
			m_pVertices[iVertex].pos.z -= m_fModelScale;
		}
		if (m_pVertices[iVertex].pos.z < _rCameraPos.z - fHalfScale)
		{
			m_pVertices[iVertex].pos.z += m_fModelScale;
		}
	}
	
	//Recreate texture based on new normal data
	//ID3D11Resource* pNormalTexture = 0;
	//D3D11_MAPPED_SUBRESOURCE* pTextureData = new D3D11_MAPPED_SUBRESOURCE;
	//GetNormalMap()->GetResource(&pNormalTexture);
	//_pDeviceContext->Map(pNormalTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, pTextureData);
	//TUCHARColour* pColourData = reinterpret_cast<TUCHARColour*>(pTextureData->pData);
	//for (int iVertex = 0; iVertex < m_iVertexCount; ++iVertex)
	//{
	//	pColourData[iVertex].r = static_cast<unsigned char>(m_pVertices[iVertex].normal.x * 255.0f);
	//	pColourData[iVertex].g = static_cast<unsigned char>(m_pVertices[iVertex].normal.z * 255.0f);
	//	pColourData[iVertex].b = static_cast<unsigned char>(m_pVertices[iVertex].normal.y * 255.0f);
	//	pColourData[iVertex].a = 255;
	//}
	//_pDeviceContext->Unmap(pNormalTexture, 0);
	//
	//pNormalTexture->Release();
	//delete pTextureData;

	CreateVertexBuffer(_pDevice);
}