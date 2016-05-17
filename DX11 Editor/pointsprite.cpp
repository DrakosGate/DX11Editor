
// Library Includes
#include <D3D11.h>

// Local Includes

// This Include
#include "pointsprite.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CPointSprite class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CPointSprite::CPointSprite()
: m_pPointSpriteVertices(0)
, m_pverts(0)
{

}

/**
*
* CPointSprite class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CPointSprite::~CPointSprite()
{
	if (m_pverts)
	{
		delete[] m_pverts;
		m_pverts = 0;
	}
	for (unsigned int iVertex = 0; iVertex < m_pPointSpriteVertices.size(); ++iVertex)
	{
		delete m_pPointSpriteVertices[iVertex];
		m_pPointSpriteVertices[iVertex] = 0;
	}
	m_pPointSpriteVertices.clear();
}

/**
*
* CPointSprite class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 device
* @return Returns true
*
*/
bool 
CPointSprite::Initialise()
{
	return true;
}
/**
*
* CPointSprite class LoadSquare
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CPointSprite::AddPointSprite(ID3D11Device* _pDevice, Math::Vector3& _rPosition, Math::Vector3& _rDirection, Math::Vector2& _rScale, Math::Colour& _rColour, float _fRotation, int _iTextureID)
{
	++m_iVertexCount;
	++m_iIndexCount;

	m_pPointSpriteVertices.push_back(new TPointSpriteVertex(_rPosition, _rDirection, _rScale, _rColour, _fRotation, _iTextureID));

	// Create the index buffer
	delete[] m_pIndices;
	m_pIndices = new unsigned int[m_iIndexCount];
	for (int iIndex = 0; iIndex < m_iIndexCount; ++iIndex)
	{
		m_pIndices[iIndex] = iIndex;
	}

	ReleaseCOM(m_pVertexBuffer);
	ReleaseCOM(m_pIndexBuffer);

	CreateVertexBuffer(_pDevice);
	CreateIndexBuffer(_pDevice);
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
CPointSprite::Draw(ID3D11DeviceContext* _pDevice)
{
	UINT stride = sizeof(TPointSpriteVertex);
	UINT offset = 0;
	_pDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	_pDevice->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_pDevice->DrawIndexed(m_iIndexCount, 0, 0);
}
/**
*
* CPointSprite class CreateModelBuffers
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CPointSprite::CreateVertexBuffer(ID3D11Device* _pDevice)
{
	delete[] m_pverts;
	m_pverts = new TPointSpriteVertex[m_pPointSpriteVertices.size()];
	for (unsigned int iVert = 0; iVert < m_pPointSpriteVertices.size(); ++iVert)
	{
		m_pverts[iVert] = *m_pPointSpriteVertices[iVert];
	}

	D3D11_BUFFER_DESC tVertexBufferDesc;
	tVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tVertexBufferDesc.ByteWidth = sizeof(TPointSpriteVertex)* m_iVertexCount;
	tVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tVertexBufferDesc.CPUAccessFlags = 0;
	tVertexBufferDesc.MiscFlags = 0;
	tVertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tVertexData;
	tVertexData.pSysMem = m_pverts;
	tVertexData.SysMemPitch = 0;
	tVertexData.SysMemSlicePitch = 0;

	_pDevice->CreateBuffer(&tVertexBufferDesc, &tVertexData, &m_pVertexBuffer);
}
/**
*
* CPointSprite class Recreates the mode buffers
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CPointSprite::RefreshBuffers(ID3D11Device* _pDevice)
{
	ReleaseCOM(m_pVertexBuffer);
	ReleaseCOM(m_pIndexBuffer);

	CreateVertexBuffer(_pDevice);
	CreateIndexBuffer(_pDevice);
}
/**
*
* CPointSprite class Returns the vertex specified by the index
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
TPointSpriteVertex*
CPointSprite::GetPointSprite(int _iIndex)
{
	return m_pPointSpriteVertices[_iIndex];
}