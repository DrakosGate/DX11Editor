//
//  File Name   :   grass.cpp
//  Description :   Code for class Grass
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <d3d11.h>

// Local Includes
#include "grassclkernel.h"
#include "resourcemanager.h"
#include "openclcontext.h"
#include "network.h"

// This Include
#include "grass.h"

// Static Variables

// Static Function Prototypes

// Implementation
Grass::Grass()
	: m_pCollisionObjects( 0 )
	//, m_pGrassCLKernel( 0 )
	, m_pGrassVelocities( 0 )
	, m_pNetwork( 0 )
{
	m_fGrassSpeed = 1.0f;
	m_fGrassStiffness = 1.0f;
}

Grass::~Grass()
{
	//SAFEDELETE( m_pGrassCLKernel );
	SAFEDELETEARRAY( m_pGrassVelocities );
}

bool
Grass::Initialise( ID3D11Device* _pDevice, ResourceManager* _pResourceManager, int _iGrassDimensions, float _fScale, Math::Vector2& _rVecTiling, Math::Colour& _rColour, int _iProcessingDivisionSize )
{
	m_iGrassDimensions = _iGrassDimensions;
	LoadTerrain( _pDevice, _iGrassDimensions, _iGrassDimensions, _fScale, _rVecTiling, _rColour );
	m_iDivisionSize = m_iVertexCount / _iProcessingDivisionSize;
	m_pGrassVelocities = new Math::Vector3[ m_iVertexCount ];

	//Create texture for grass normal data
	int iCurrentPixel = 0;
	std::string sTextureName = "grassnormals";
	TUCHARColour* pTerrainTexture = new TUCHARColour[ _iGrassDimensions * _iGrassDimensions ];
	for( int iHeight = 0; iHeight < _iGrassDimensions; ++iHeight )
	{
		for( int iWidth = 0; iWidth < _iGrassDimensions; ++iWidth )
		{
			m_pVertices[ iCurrentPixel ].normal += Math::Vector3( rand() % 100 * 0.01f, 0.0f, rand() % 100 * 0.01f );
			m_pGrassVelocities[ iCurrentPixel ] = Math::Vector3( 0.0f, 0.0f, 0.0f );

			pTerrainTexture[ iCurrentPixel ].r = static_cast<unsigned char>( m_pVertices[ iCurrentPixel ].normal.x * 255.0f );
			pTerrainTexture[ iCurrentPixel ].g = static_cast<unsigned char>( m_pVertices[ iCurrentPixel ].normal.z * 255.0f );
			pTerrainTexture[ iCurrentPixel ].b = static_cast<unsigned char>( m_pVertices[ iCurrentPixel ].normal.y * 255.0f );
			pTerrainTexture[ iCurrentPixel ].a = 255;
			++iCurrentPixel;
		}
	}
	//SetNormalMap(_pResourceManager->CreateTextureFromData(_pDevice, reinterpret_cast<unsigned char*>(pTerrainTexture), sTextureName, _iTerrainWidth, _iTerrainHeight));
	delete[] pTerrainTexture;

	//KEY AREA: Grass OpenCL Kernel setup
	m_fGrassSpeed = 30.0f;
	m_fGrassStiffness = 2.0f;
	//m_pGrassCLKernel = new CGrassCLKernel();
	//m_pGrassCLKernel->CreateBuffers( _pCLKernel, this, m_pCollisionObjects );
	//_pCLKernel->LoadProgram( m_pGrassCLKernel->GetCLProgram(), m_pGrassCLKernel->GetCLKernel(), "Assets/OpenCLKernels/grass.cl", "ProcessGrass" );

	m_pNetwork = Network::GetInstance();

	return true;
}

void
Grass::ProcessDistrubuted( float _fDeltaTime )
{
	m_pNetwork->ProcessGrass( _fDeltaTime );
}

void
Grass::ProcessOpenCL( float _fDeltaTime )
{
	//m_pGrassCLKernel->SendDataToGPU( _pCLKernel, this, m_pCollisionObjects, _fDeltaTime );
	//_pCLKernel->Run( m_pGrassCLKernel->GetCLKernel() );
	//m_pGrassCLKernel->RetrieveOpenCLResults( _pCLKernel, this );
}

void
Grass::SendCollisionData( std::vector<RenderEntity*>* _pCollisionObjects )
{
	m_pCollisionObjects = _pCollisionObjects;
}

void
Grass::ProcessGrassSection( int _iSection, float _fDeltaTime )
{
	//KEY AREA: Multithreaded grass processing function
	if( m_pCollisionObjects )
	{
		int iStartingPosition = _iSection * m_iDivisionSize;
		for( int iBlade = 0; iBlade < m_iDivisionSize; ++iBlade )
		{
			int iCurrentVertex = iStartingPosition + iBlade;
			if( iCurrentVertex < m_iVertexCount )
			{
				//Avoid entities
				Math::Vector3 vecToEntity;
				float fAvoidanceRange = 1.0f;
				unsigned int iNumEntities = m_pCollisionObjects->size();
				for( unsigned int iEntity = 0; iEntity < iNumEntities; ++iEntity )
				{
					if( ( *m_pCollisionObjects )[ iEntity ]->DoDraw() )
					{
						fAvoidanceRange = ( ( *m_pCollisionObjects )[ iEntity ]->GetRadius() * ( *m_pCollisionObjects )[ iEntity ]->GetRadius() * ( *m_pCollisionObjects )[ iEntity ]->GetScale().x * 0.5f );

						vecToEntity = ( *m_pCollisionObjects )[ iEntity ]->GetPosition() - m_pVertices[ iCurrentVertex ].pos;
						float fDistanceToEntity = vecToEntity.LengthSq();
						if( fDistanceToEntity < fAvoidanceRange )
						{
							m_pGrassVelocities[ iCurrentVertex ] -= ( vecToEntity * ( fAvoidanceRange - fDistanceToEntity ) ) * 2.0f * m_fGrassSpeed * _fDeltaTime;
						}
					}
				}
				if( m_pVertices[ iCurrentVertex ].normal.y < 0.5f )
				{
					m_pVertices[ iCurrentVertex ].normal.y = 0.5f;
				}
			}
		}
	}
}

void
Grass::RecreateGrassMesh( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, Math::Vector3& _rCameraPos, std::vector<RenderEntity*>& _pEntities, float _fDeltaTime )
{
	float fHalfScale = m_fModelScale * 0.5f;
	float fGrassSpringiness = 1.5f;
	m_fGrassStiffness = 0.5f;

	//Delete and recreate vertex buffer data
	m_pVertexBuffer->Release();
	Math::Vector3 vecNormalTarget;
	Math::Vector3 vecWind = Math::Vector3( 0.1f, 0.0f, 0.0f );

	for( int iVertex = 0; iVertex < m_iVertexCount; ++iVertex )
	{
		vecNormalTarget = Math::Vector3( 0.0f, 1.0f, 0.0f ) * m_fGrassStiffness;
		if( iVertex > 0 && iVertex < m_iVertexCount - 1 )
		{
			vecNormalTarget += ( m_pVertices[ iVertex - 1 ].normal - m_pVertices[ iVertex ].normal ) + ( m_pVertices[ iVertex + 1 ].normal - m_pVertices[ iVertex ].normal ) * fGrassSpringiness;
		}
		if( iVertex > m_iGrassDimensions && iVertex < m_iVertexCount - m_iGrassDimensions - 1 )
		{
			vecNormalTarget += ( m_pVertices[ iVertex - m_iGrassDimensions ].normal - m_pVertices[ iVertex ].normal ) + ( m_pVertices[ iVertex + m_iGrassDimensions ].normal - m_pVertices[ iVertex ].normal ) * fGrassSpringiness;
		}
		Math::Vector3 vecWindDir;
		vecWindDir = Math::Normalise( vecWind );
		float fWindPower = 1.0f - fabs( Math::Dot( vecWindDir, m_pVertices[ iVertex ].normal ) );
		//Move to target location
		m_pGrassVelocities[ iVertex ] += Math::Vector3( vecNormalTarget - m_pVertices[ iVertex ].normal ) * _fDeltaTime;
		//Apply wind force
		m_pGrassVelocities[ iVertex ] += vecWind * fWindPower * _fDeltaTime;

		m_pVertices[ iVertex ].normal += m_pGrassVelocities[ iVertex ] * _fDeltaTime;
		m_pGrassVelocities[ iVertex ] -= m_pGrassVelocities[ iVertex ] * 1.5f * _fDeltaTime;

		//Width
		if( m_pVertices[ iVertex ].pos.x > _rCameraPos.x + fHalfScale )
		{
			m_pVertices[ iVertex ].pos.x -= m_fModelScale;
		}
		if( m_pVertices[ iVertex ].pos.x < _rCameraPos.x - fHalfScale )
		{
			m_pVertices[ iVertex ].pos.x += m_fModelScale;
		}
		//Height
		if( m_pVertices[ iVertex ].pos.z > _rCameraPos.z + fHalfScale )
		{
			m_pVertices[ iVertex ].pos.z -= m_fModelScale;
		}
		if( m_pVertices[ iVertex ].pos.z < _rCameraPos.z - fHalfScale )
		{
			m_pVertices[ iVertex ].pos.z += m_fModelScale;
		}
		m_pVertices[ iVertex ].normal = Math::Normalise( m_pVertices[ iVertex ].normal );
	}

	CreateVertexBuffer( _pDevice );
}

int
Grass::GetDimensionSize() const
{
	return m_iGrassDimensions;
}