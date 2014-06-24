//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   boundingbox.cpp
//  Description :   Code for CBoundingBox 
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <vector>

// Local Includes
#include "Vertex.h"

// This Include
#include "boundingbox.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CBoundingBox class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CBoundingBox::CBoundingBox()
{
}

/**
*
* CBoundingBox class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CBoundingBox::~CBoundingBox()
{

}


/**
*
* CBoundingBox class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 device
* @return Returns true
*
*/
bool 
CBoundingBox::Initialise(TVertex* _pObjectVertices, int _iVertexCount)
{
	float fFloatMax = 10000000.0f;
	D3DXVECTOR3 vecMin = D3DXVECTOR3(fFloatMax, fFloatMax, fFloatMax);
	D3DXVECTOR3 vecMax = D3DXVECTOR3(-fFloatMax, -fFloatMax, -fFloatMax);

	for(int iVertex = 0; iVertex < _iVertexCount; ++iVertex)
	{
		//X Position
		if(_pObjectVertices[iVertex].pos.x < vecMin.x)
		{
			vecMin.x = _pObjectVertices[iVertex].pos.x;
		}
		if(_pObjectVertices[iVertex].pos.y > vecMax.x)
		{
			vecMax.x = _pObjectVertices[iVertex].pos.x;
		}
		//Y Position
		if(_pObjectVertices[iVertex].pos.y < vecMin.y)
		{
			vecMin.y = _pObjectVertices[iVertex].pos.y;
		}
		if(_pObjectVertices[iVertex].pos.y > vecMax.y)
		{
			vecMax.y = _pObjectVertices[iVertex].pos.y;
		}
		//Z Position
		if(_pObjectVertices[iVertex].pos.z < vecMin.z)
		{
			vecMin.z = _pObjectVertices[iVertex].pos.z;
		}
		if(_pObjectVertices[iVertex].pos.z > vecMax.z)
		{
			vecMax.z = _pObjectVertices[iVertex].pos.z;
		}
	}

	m_vecScale.x = (vecMax.x - vecMin.x) * 0.5f;
	m_vecScale.y = (vecMax.y - vecMin.y) * 0.5f;
	m_vecScale.z = (vecMax.z - vecMin.z) * 0.5f;

	m_tBoundingBox.fLeft =	-m_vecScale.x;
	m_tBoundingBox.fRight =	m_vecScale.x;
	m_tBoundingBox.fTop =	m_vecScale.y;
	m_tBoundingBox.fBottom = -m_vecScale.y;

	m_vecPosition = m_vecScale * 0.5f;

	if(m_vecScale.x > m_vecScale.y)
	{
		if(m_vecScale.x > m_vecScale.z)
		{
			m_fRadius = m_vecScale.x * 2.0f;
		}
		else
		{
			m_fRadius = m_vecScale.z * 2.0f;
		}
	}
	else if(m_vecScale.z > m_vecScale.y)
	{
		if(m_vecScale.z > m_vecScale.x)
		{
			m_fRadius = m_vecScale.z * 2.0f;
		}
		else
		{
			m_fRadius = m_vecScale.x * 2.0f;
		}
	}
	else
	{
		if(m_vecScale.y > m_vecScale.z)
		{
			m_fRadius = m_vecScale.y * 2.0f;
		}
		else
		{
			m_fRadius = m_vecScale.z * 2.0f;
		}
	}

	return true;
}
/**
*
* CBoundingBox class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 device
* @return Returns true
*
*/
bool 
CBoundingBox::Initialise(TAnimatedVertex* _pObjectVertices, int _iVertexCount)
{
	float fFloatMax = 10000000.0f;
	D3DXVECTOR3 vecMin = D3DXVECTOR3(fFloatMax, fFloatMax, fFloatMax);
	D3DXVECTOR3 vecMax = D3DXVECTOR3(-fFloatMax, -fFloatMax, -fFloatMax);

	for(int iVertex = 0; iVertex < _iVertexCount; ++iVertex)
	{
		//X Position
		if(_pObjectVertices[iVertex].pos.x < vecMin.x)
		{
			vecMin.x = _pObjectVertices[iVertex].pos.x;
		}
		if(_pObjectVertices[iVertex].pos.y > vecMax.x)
		{
			vecMax.x = _pObjectVertices[iVertex].pos.x;
		}
		//Y Position
		if(_pObjectVertices[iVertex].pos.y < vecMin.y)
		{
			vecMin.y = _pObjectVertices[iVertex].pos.y;
		}
		if(_pObjectVertices[iVertex].pos.y > vecMax.y)
		{
			vecMax.y = _pObjectVertices[iVertex].pos.y;
		}
		//Z Position
		if(_pObjectVertices[iVertex].pos.z < vecMin.z)
		{
			vecMin.z = _pObjectVertices[iVertex].pos.z;
		}
		if(_pObjectVertices[iVertex].pos.z > vecMax.z)
		{
			vecMax.z = _pObjectVertices[iVertex].pos.z;
		}
	}

	m_vecScale.x = (vecMax.x - vecMin.x) * 0.5f;
	m_vecScale.y = (vecMax.y - vecMin.y) * 0.5f;
	m_vecScale.z = (vecMax.z - vecMin.z) * 0.5f;

	m_tBoundingBox.fLeft =	-m_vecScale.x;
	m_tBoundingBox.fRight =	m_vecScale.x;
	m_tBoundingBox.fTop =	m_vecScale.y;
	m_tBoundingBox.fBottom = -m_vecScale.y;

	m_vecPosition = m_vecScale * 0.5f;

	if(m_vecScale.x > m_vecScale.y)
	{
		if(m_vecScale.x > m_vecScale.z)
		{
			m_fRadius = m_vecScale.x * 2.0f;
		}
		else
		{
			m_fRadius = m_vecScale.z * 2.0f;
		}
	}
	else if(m_vecScale.z > m_vecScale.y)
	{
		if(m_vecScale.z > m_vecScale.x)
		{
			m_fRadius = m_vecScale.z * 2.0f;
		}
		else
		{
			m_fRadius = m_vecScale.x * 2.0f;
		}
	}
	else
	{
		if(m_vecScale.y > m_vecScale.z)
		{
			m_fRadius = m_vecScale.y * 2.0f;
		}
		else
		{
			m_fRadius = m_vecScale.z * 2.0f;
		}
	}

	return true;
}
/**
*
* CBoundingBox class GetRadius
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns box radius
*
*/
float 
CBoundingBox::GetRadius() const
{
	return m_fRadius;
}
/**
*
* CBoundingBox class GetRect
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns rectangle bounding box
*
*/
TBoundingBox* 
CBoundingBox::GetRect()
{
	return &m_tBoundingBox;
}
