//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   model.cpp
//  Description :   Code for class CPerformanceGraph
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <D3D11.h>

// Local Includes

// This Include
#include "performancegraph.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CPerformanceGraph class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CPerformanceGraph::CPerformanceGraph()
: m_pNodeValues(0)
, m_fMinRange(0.0f)
, m_fMaxRange(0.000001f)
, m_fScaleOffset(0.0f)
, m_iNumNodes(0)
{

}

/**
*
* CPerformanceGraph class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CPerformanceGraph::~CPerformanceGraph()
{
	SAFEDELETEARRAY(m_pNodeValues);
}
/**
*
* CPerformanceGraph class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 device
* @param _iNumNodes Number of nodes in the graph
* @return Returns true
*
*/
bool 
CPerformanceGraph::Initialise(ID3D11Device* _pDevice, D3DXVECTOR3& _rPosition, D3DXVECTOR3& _rScale, int _iNumNodes)
{
	m_fScaleOffset = _rScale.x / static_cast<float>(_iNumNodes);
	m_iNumNodes = _iNumNodes;
	m_vecGraphPos = D3DXVECTOR3(_rPosition.x - WINDOW_WIDTH * 0.5f, WINDOW_HEIGHT * 0.5f - _rPosition.y, 0.0f);
	m_vecGraphScale = _rScale;
	m_pNodeValues = new float[m_iNumNodes];
	
	for (int iNode = 0; iNode < _iNumNodes; ++iNode)
	{
		m_pNodeValues[iNode] = 0.0f;
		AddPointSprite(_pDevice, _rPosition + D3DXVECTOR3(m_fScaleOffset * iNode, 0.0f, 0.0f), D3DXVECTOR3(10.0f, 0.0f, 0.0f), D3DXVECTOR2(m_fScaleOffset, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f), 0.0f, 0);
	}
	//Add background sprite
	AddPointSprite(_pDevice, m_vecGraphPos, D3DXVECTOR3(10.0f, 0.0f, 0.0f), D3DXVECTOR2(_rScale.x, _rScale.y), D3DXCOLOR(0.2f, 0.2f, 1.0f, 0.3f), 0.0f, 0);

	return true;
}
/**
*
* CPointSprite class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CPerformanceGraph::Draw(ID3D11DeviceContext* _pDevice)
{
	UINT stride = sizeof(TPointSpriteVertex);
	UINT offset = 0;
	_pDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	_pDevice->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_pDevice->DrawIndexed(m_iIndexCount, 0, 0);
}
/**
*
* CPerformanceGraph class Set the vertical range of the graph
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CPerformanceGraph::SetGraphRange(float _fMinRange, float _fMaxRange)
{
	if (m_fMinRange > _fMinRange)
	{
		m_fMinRange = _fMinRange;
	}
	if (m_fMaxRange < _fMaxRange)
	{
		m_fMaxRange = _fMaxRange;
	}
	m_fHeightPercentage = 1.0f / (m_fMaxRange - m_fMinRange);
}
/**
*
* CPerformanceGraph class Adds a node to the graph
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CPerformanceGraph::AddNode(ID3D11Device* _pDevice, float _fValue)
{
	//Ensure value is in range
	float fValue = _fValue;
	if (fValue < m_fMinRange)
	{
		fValue = m_fMinRange;
	}
	if (fValue > m_fMaxRange)
	{
		fValue = m_fMaxRange;
	}

	//Pass value to previous node
	for (int iNode = 0; iNode < m_iNumNodes - 1; ++iNode)
	{
		m_pNodeValues[iNode] = m_pNodeValues[iNode + 1];
	}
	//Add new value to node graph
	m_pNodeValues[m_iNumNodes - 1] = _fValue;

	//Recreate buffers
	for (int iVertex = 0; iVertex < m_iNumNodes; ++iVertex)
	{
		float fHeightOffset = ((m_pNodeValues[iVertex] - m_fMinRange) * m_fHeightPercentage) * m_vecGraphScale.y;
		m_pPointSpriteVertices[iVertex]->pos = D3DXVECTOR3(m_vecGraphPos.x + (m_fScaleOffset * iVertex), m_vecGraphPos.y - m_vecGraphScale.y + fHeightOffset, m_vecGraphPos.z);
		m_pPointSpriteVertices[iVertex]->scale.y = m_pPointSpriteVertices[iVertex]->pos.y - (m_vecGraphPos.y - m_vecGraphScale.y);
	}
	RefreshBuffers(_pDevice);
}
float
CPerformanceGraph::GetMin() const
{
	return m_fMinRange;
}
float
CPerformanceGraph::GetMax() const
{
	return m_fMaxRange;
}