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
CGrass::Initialise(ID3D11Device* _pDevice, CResourceManager* _pResourceManager, int _iTerrainWidth, int _iTerrainHeight, float _fScale, D3DXVECTOR2& _rVecTiling, D3DXCOLOR& _rColour)
{
	LoadTerrain(_pDevice, _iTerrainWidth, _iTerrainHeight, _fScale, _rVecTiling, _rColour);

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
	SetNormalMap(_pResourceManager->CreateTextureFromData(_pDevice, reinterpret_cast<unsigned char*>(pTerrainTexture), sTextureName, _iTerrainWidth, _iTerrainHeight));
	delete[] pTerrainTexture;

	return true;
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
		m_pVertices[iVertex].normal += (vecNormalTarget - m_pVertices[iVertex].normal) * m_fGrassSpeed * _fDeltaTime;
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
	

		//Avoid entities
		D3DXVECTOR3 vecToEntity;
		float fAvoidanceRange = 1.0f;
		unsigned int iNumEntities = _pEntities.size();
		for (unsigned int iEntity = 0; iEntity < iNumEntities; ++iEntity)
		{
			fAvoidanceRange = _pEntities[iEntity]->GetRadius();
	
			vecToEntity = _pEntities[iEntity]->GetPosition() - m_pVertices[iVertex].pos;
			float fDistanceToEntity = D3DXVec3LengthSq(&vecToEntity);
			if (fDistanceToEntity < fAvoidanceRange)
			{
				m_pVertices[iVertex].normal -= (vecToEntity * (fAvoidanceRange - fDistanceToEntity) * 4.0f) * m_fGrassSpeed * _fDeltaTime;
			}
		}
		if(m_pVertices[iVertex].normal.y < 0.5f)
		{
			m_pVertices[iVertex].normal.y = 0.5f;
		}
		D3DXVec3Normalize(&m_pVertices[iVertex].normal, &m_pVertices[iVertex].normal);
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