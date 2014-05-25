//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   tile.cpp
//  Description :   Code for Class tile
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes

// Local Includes
#include "model.h"
#include "boundingbox.h"

// This Include
#include "prefab.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CPrefab class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CPrefab::CPrefab()
: m_pModel(0)
, m_eTileType(TILE_INVALID)
, m_fBoundingBoxPercentage(0.0f)
, m_bIsCollidable(false)
{	
	m_vecScale.x = 1.0f;
	m_vecScale.y = 1.0f;
	m_vecScale.z = 1.0f;
}

CPrefab::~CPrefab()
{
	if (m_pBoundingBox)
	{
		delete m_pBoundingBox;
		m_pBoundingBox = 0;
	}
}
bool 
CPrefab::Initialise(ID3D11Device* _pDevice, float _fScale)
{
	m_vecScale.x = _fScale;
	m_vecScale.y = _fScale;
	m_vecScale.z = _fScale;

	if (!m_pBoundingBox)
	{
		m_pBoundingBox = new CBoundingBox();
		m_fBoundingBoxPercentage = 1.0f; //Full bounding box size
	}
	return true;
}
/**
*
* CPrefab class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 Device
*
*/
void 
CPrefab::Draw(ID3D11DeviceContext* _pDevice)
{
	m_pModel->Draw(_pDevice);
}
void 
CPrefab::SetModel(CModel* _pModel)
{
	m_pModel = _pModel;
	
	TBoundingBox* pBoundingBox = m_pBoundingBox->GetRect();
	TBoundingBox* pSpriteBox = m_pModel->GetBoundingBox()->GetRect();
	pBoundingBox->fLeft		= pSpriteBox->fLeft * m_vecScale.x * m_fBoundingBoxPercentage;
	pBoundingBox->fRight	= pSpriteBox->fRight * m_vecScale.x * m_fBoundingBoxPercentage;
	pBoundingBox->fTop		= pSpriteBox->fTop * m_vecScale.y * m_fBoundingBoxPercentage;
	pBoundingBox->fBottom	= pSpriteBox->fBottom * m_vecScale.y * m_fBoundingBoxPercentage;
	m_fRadius = m_pModel->GetBoundingBox()->GetRadius();
}
void 
CPrefab::SetTileType(ETileType _eTileType)
{
	m_eTileType = _eTileType;
}
ETileType 
CPrefab::GetTileType() const
{
	return m_eTileType;
}
bool 
CPrefab::IsCollidable() const
{
	return m_bIsCollidable;
}
void 
CPrefab::ToggleIsCollidable(bool _bIsCollidable)
{
	m_bIsCollidable = _bIsCollidable;
}
bool 
CPrefab::CheckPointCollision(D3DXVECTOR3& _rPoint)
{
	TBoundingBox* pBox = m_pBoundingBox->GetRect();
	return (	pBox->fLeft + m_vecPosition.x < _rPoint.x &&
				pBox->fRight + m_vecPosition.x > _rPoint.x &&
				pBox->fBottom + m_vecPosition.y < _rPoint.y &&
				pBox->fTop + m_vecPosition.y > _rPoint.y);
}
void 
CPrefab::SetScale(D3DXVECTOR3& _rVecScale)
{
	m_vecScale = _rVecScale;

	//Recalculate bounding box
	if(m_pModel)
	{
		TBoundingBox* pSpriteBox = m_pModel->GetBoundingBox()->GetRect();
		m_pBoundingBox->GetRect()->fLeft	= pSpriteBox->fLeft * m_vecScale.x * m_fBoundingBoxPercentage;
		m_pBoundingBox->GetRect()->fRight	= pSpriteBox->fRight * m_vecScale.x * m_fBoundingBoxPercentage;
		m_pBoundingBox->GetRect()->fTop		= pSpriteBox->fTop * m_vecScale.y * m_fBoundingBoxPercentage;
		m_pBoundingBox->GetRect()->fBottom	= pSpriteBox->fBottom * m_vecScale.y * m_fBoundingBoxPercentage;
	}
}