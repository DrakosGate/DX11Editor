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
* Prefab class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
Prefab::Prefab()
: m_pModel( nullptr )
, m_eTileType(TILE_INVALID)
, m_fBoundingBoxPercentage(0.0f)
, m_bIsCollidable(false)
{	
	m_vecScale.x = 1.0f;
	m_vecScale.y = 1.0f;
	m_vecScale.z = 1.0f;
}

Prefab::~Prefab()
{
	if (m_pBoundingBox)
	{
		delete m_pBoundingBox;
		m_pBoundingBox = 0;
	}
}
bool 
Prefab::Initialise(ID3D11Device* _pDevice, float _fScale)
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
* Prefab class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 Device
*
*/
void 
Prefab::Draw(ID3D11DeviceContext* _pDevice)
{
	m_pModel->Draw(_pDevice);
}
void 
Prefab::SetModel(Model* _pModel)
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
Prefab::SetTileType(ETileType _eTileType)
{
	m_eTileType = _eTileType;
}
ETileType 
Prefab::GetTileType() const
{
	return m_eTileType;
}
bool 
Prefab::IsCollidable() const
{
	return m_bIsCollidable;
}
void 
Prefab::ToggleIsCollidable(bool _bIsCollidable)
{
	m_bIsCollidable = _bIsCollidable;
}
bool 
Prefab::CheckPointCollision(Math::Vector3& _rPoint)
{
	TBoundingBox* pBox = m_pBoundingBox->GetRect();
	return (	pBox->fLeft + m_vecPosition.x < _rPoint.x &&
				pBox->fRight + m_vecPosition.x > _rPoint.x &&
				pBox->fBottom + m_vecPosition.y < _rPoint.y &&
				pBox->fTop + m_vecPosition.y > _rPoint.y);
}
void 
Prefab::SetScale(Math::Vector3& _rVecScale)
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