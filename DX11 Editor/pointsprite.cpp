
// Library Includes
#include <D3D11.h>

// Local Includes

// This Include
#include "pointsprite.h"

// Static Variables

// Static Function Prototypes

// Implementation
PointSprite::PointSprite()
	: m_pverts( nullptr )
{

}

PointSprite::~PointSprite()
{
	if( m_pverts )
	{
		delete[] m_pverts;
		m_pverts = 0;
	}
	for( unsigned int iVertex = 0; iVertex < m_pPointSpriteVertices.size(); ++iVertex )
	{
		delete m_pPointSpriteVertices[ iVertex ];
		m_pPointSpriteVertices[ iVertex ] = 0;
	}
	m_pPointSpriteVertices.clear();
}

bool
PointSprite::Initialise()
{
	return true;
}

void
PointSprite::AddPointSprite( ID3D11Device* _pDevice, Math::Vector3& _rPosition, Math::Vector3& _rDirection, Math::Vector2& _rScale, Math::Colour& _rColour, float _fRotation, int _iTextureID )
{
	++m_iVertexCount;
	++m_iIndexCount;

	m_pPointSpriteVertices.push_back( new TPointSpriteVertex( _rPosition, _rDirection, _rScale, _rColour, _fRotation, _iTextureID ) );

	// Create the index buffer
	delete[] m_pIndices;
	m_pIndices = new unsigned int[ m_iIndexCount ];
	for( int iIndex = 0; iIndex < m_iIndexCount; ++iIndex )
	{
		m_pIndices[ iIndex ] = iIndex;
	}

	ReleaseCOM( m_pVertexBuffer );
	ReleaseCOM( m_pIndexBuffer );

	CreateVertexBuffer( _pDevice );
	CreateIndexBuffer( _pDevice );
}

void
PointSprite::Draw( ID3D11DeviceContext* _pDevice )
{
	UINT stride = sizeof( TPointSpriteVertex );
	UINT offset = 0;
	_pDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	_pDevice->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	_pDevice->DrawIndexed( m_iIndexCount, 0, 0 );
}

void
PointSprite::CreateVertexBuffer( ID3D11Device* _pDevice )
{
	delete[] m_pverts;
	m_pverts = new TPointSpriteVertex[ m_pPointSpriteVertices.size() ];
	for( unsigned int iVert = 0; iVert < m_pPointSpriteVertices.size(); ++iVert )
	{
		m_pverts[ iVert ] = *m_pPointSpriteVertices[ iVert ];
	}

	D3D11_BUFFER_DESC tVertexBufferDesc;
	tVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tVertexBufferDesc.ByteWidth = sizeof( TPointSpriteVertex )* m_iVertexCount;
	tVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tVertexBufferDesc.CPUAccessFlags = 0;
	tVertexBufferDesc.MiscFlags = 0;
	tVertexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tVertexData;
	tVertexData.pSysMem = m_pverts;
	tVertexData.SysMemPitch = 0;
	tVertexData.SysMemSlicePitch = 0;

	_pDevice->CreateBuffer( &tVertexBufferDesc, &tVertexData, &m_pVertexBuffer );
}

void
PointSprite::RefreshBuffers( ID3D11Device* _pDevice )
{
	ReleaseCOM( m_pVertexBuffer );
	ReleaseCOM( m_pIndexBuffer );

	CreateVertexBuffer( _pDevice );
	CreateIndexBuffer( _pDevice );
}

TPointSpriteVertex*
PointSprite::GetPointSprite( int _iIndex )
{
	return m_pPointSpriteVertices[ _iIndex ];
}