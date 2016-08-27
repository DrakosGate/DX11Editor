
// Library Includes
#include <D3D11.h>
#include <vector>

// Local Includes
#include "vertex.h"
#include "boundingbox.h"
#include "shader.h"
#include "camera.h"

// This Include
#include "renderentity.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* RenderEntity class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
RenderEntity::RenderEntity()
	: m_pVertexBuffer( 0 )
	, m_pIndexBuffer( 0 )
	, m_pVertices( 0 )
	, m_pIndices( 0 )
	, m_pObjectShader( 0 )
	, m_pBoundingBox( 0 )
	, m_iVertexCount( 0 )
	, m_iIndexCount( 0 )
	, m_bIsShadowed( false )
	, m_bIsTransparent( false )
	, m_bIsBillboarded( false )
	, m_bDoDraw( true )
	, m_fRadius( 0.0f )
	, m_pDiffuseMap( 0 )
	, m_pNormalMap( 0 )
	, m_pNode( 0 )
{
	m_matWorld = Math::MatrixIdentity();
	m_vecPosition *= 0.0f;
	m_vecScale = Math::Vector3( 1.0f, 1.0f, 1.0f );
	m_vecRotation *= 0.0f;

	m_vecLook *= 0.0f;
	m_vecUp *= 0.0f;
	m_vecRight *= 0.0f;

	m_sEntityType = "NONE";
}

/**
*
* RenderEntity class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
RenderEntity::~RenderEntity()
{
	if( m_pNode )
	{
		delete m_pNode;
		m_pNode = 0;
	}
	if( m_pBoundingBox )
	{
		delete m_pBoundingBox;
		m_pBoundingBox = 0;
	}
	if( m_pVertices )
	{
		delete[] m_pVertices;
		m_pVertices = 0;
	}
	if( m_pIndices )
	{
		delete[] m_pIndices;
		m_pIndices = 0;
	}
	m_pDiffuseMap = 0;
	m_pNormalMap = 0;
	ReleaseCOM( m_pVertexBuffer );
	ReleaseCOM( m_pIndexBuffer );
}
/**
*
* RenderEntity class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 device
* @return Returns true
*
*/
bool
RenderEntity::Initialise( ID3D11Device* _pDevice, float _fScale )
{
	//Render entity will never be created
	return true;
}
/**
*
* RenderEntity class Update
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Time elapsed
*
*/
void
RenderEntity::Process( float _fDeltaTime, Math::Matrix* _pParentMatrix )
{
	if( DoDraw() )
	{
		if( m_bIsBillboarded == false )
		{
			//Calculate world matrix
			m_quatRot = Math::QuaternionRotationYawPitchRoll( m_vecRotation.y, m_vecRotation.x, m_vecRotation.z );
			m_matWorld = Math::MatrixTransformation(	nullptr,
														nullptr,
														&m_vecScale,
														nullptr,
														&m_quatRot,
														&Math::Vector4( m_vecPosition.x, m_vecPosition.y, m_vecPosition.z, 1.0f ) );
			//Check if this matrix must be multiplied by the parent matrix
			if( _pParentMatrix )
			{
				m_matWorld = m_matWorld * *_pParentMatrix;
			}

			//Process all children of this entity
			if( m_pNode )
			{
				if( m_pNode->pEntity )
				{
					for( unsigned int iChild = 0; iChild < m_pNode->vecChildren.size(); ++iChild )
					{
						m_pNode->vecChildren[ iChild ]->pEntity->Process( _fDeltaTime, &m_matWorld );
					}
					//Process all lights attached to this entity
					for( unsigned int iLight = 0; iLight < m_pNode->vecLights.size(); ++iLight )
					{
						CLight* pCurrentLight = m_pNode->vecLights[ iLight ];
						pCurrentLight->SetPosition( GetPosition() + pCurrentLight->GetOffset() );
						pCurrentLight->SetDirection( GetForward() );
					}
				}
			}
		}
	}
}
/**
*
* RenderEntity class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
RenderEntity::Draw( ID3D11DeviceContext* _pDevice )
{
	UINT stride = sizeof( TVertex::SourceType );
	UINT offset = 0;
	_pDevice->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );
	_pDevice->IASetIndexBuffer( m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );
	_pDevice->DrawIndexed( m_iIndexCount, 0, 0 );
}
/**
*
* RenderEntity class CreateModelBuffers
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
RenderEntity::CreateVertexBuffer( ID3D11Device* _pDevice )
{
	D3D11_BUFFER_DESC tVertexBufferDesc;
	tVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tVertexBufferDesc.ByteWidth = sizeof( TVertex::SourceType )* m_iVertexCount;
	tVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tVertexBufferDesc.CPUAccessFlags = 0;
	tVertexBufferDesc.MiscFlags = 0;
	tVertexBufferDesc.StructureByteStride = 0;

	auto sourceData = new TVertex::SourceType[ m_iVertexCount ];
	for( int index = 0; index < m_iVertexCount; ++index )
		sourceData[ index ] = m_pVertices[ index ].GetSourceType();
	
	D3D11_SUBRESOURCE_DATA tVertexData;
	tVertexData.pSysMem = sourceData;
	tVertexData.SysMemPitch = 0;
	tVertexData.SysMemSlicePitch = 0;

	_pDevice->CreateBuffer( &tVertexBufferDesc, &tVertexData, &m_pVertexBuffer );

	SAFEDELETEARRAY( sourceData );
}
/**
*
* RenderEntity class CreateModelBuffers
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
RenderEntity::CreateIndexBuffer( ID3D11Device* _pDevice )
{
	D3D11_BUFFER_DESC tIndexBufferDesc;
	tIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tIndexBufferDesc.ByteWidth = sizeof( unsigned int ) * m_iIndexCount;
	tIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIndexBufferDesc.CPUAccessFlags = 0;
	tIndexBufferDesc.MiscFlags = 0;
	tIndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tIndexBufferData;
	tIndexBufferData.pSysMem = m_pIndices;
	tIndexBufferData.SysMemPitch = 0;
	tIndexBufferData.SysMemSlicePitch = 0;

	_pDevice->CreateBuffer( &tIndexBufferDesc, &tIndexBufferData, &m_pIndexBuffer );
}
/**
*
* RenderEntity class Creates an Entity Node for this object
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pParentNode This objects parent
* @return Returns pointer to new node
*
*/
TEntityNode*
RenderEntity::CreateNode( TEntityNode* _pParentNode )
{
	m_pNode = new TEntityNode( this, _pParentNode );
	if( _pParentNode )
	{
		_pParentNode->vecChildren.push_back( m_pNode );
	}
	else
	{
		//This is the root node
	}
	return m_pNode;
}
/**
*
* RenderEntity class GetNode Returns this objects "Entity Node"
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns entity node
*
*/
TEntityNode*
RenderEntity::GetNode() const
{
	return m_pNode;
}
/**
*
* RenderEntity class GetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns position
*
*/
Math::Vector3&
RenderEntity::GetPosition()
{
	return m_vecPosition;
}
/**
*
* RenderEntity class GetRotation
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns rotation
*
*/
Math::Vector3&
RenderEntity::GetRotation()
{
	return m_vecRotation;
}
/**
*
* RenderEntity class GetForward
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns Forward
*
*/
Math::Vector3&
RenderEntity::GetForward()
{
	return m_vecLook;
}
/**
*
* RenderEntity class GetWorld
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns world matrix
*
*/
Math::Matrix&
RenderEntity::GetWorld()
{
	return m_matWorld;
}
/**
*
* RenderEntity class GetEntityType
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns type of this entity
*
*/
std::string&
RenderEntity::GetEntityType()
{
	return m_sEntityType;
}
/**
*
* RenderEntity class SetRotation
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rVecRotation New rotation vector
*
*/
void
RenderEntity::SetRotation( Math::Vector3& _rVecRotation )
{
	m_vecRotation = _rVecRotation;
}
/**
*
* RenderEntity class SetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rVecPosition New position vector
*
*/
void
RenderEntity::SetPosition( Math::Vector3& _rVecPosition )
{
	m_vecPosition = _rVecPosition;
}
/**
*
* RenderEntity class SetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rVecPosition New position vector
*
*/
void
RenderEntity::SetForward( Math::Vector3& _rVecForward )
{
	m_vecRotation.y = atan2( _rVecForward.x, _rVecForward.z );
	m_vecLook = _rVecForward;
}
/**
*
* RenderEntity class SetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rVecPosition New position vector
*
*/
void
RenderEntity::SetEntityType( std::string& _sType )
{
	m_sEntityType = _sType;
}

/**
*
* RenderEntity class SetWorldMatrix
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rWorld New World matrix
*
*/
void
RenderEntity::SetWorldMatrix( Math::Matrix& _rWorld )
{
	m_matWorld = _rWorld;
}
void
RenderEntity::SetScale( Math::Vector3& _rVecScale )
{
	m_vecScale = _rVecScale;
}
void
RenderEntity::SetLocalScale( Math::Vector3& _rLocalScale )
{
	m_vecLocalScale = _rLocalScale;
}
Math::Vector3&
RenderEntity::GetScale()
{
	return m_vecScale;
}
Math::Vector3&
RenderEntity::GetLocalScale()
{
	return m_vecLocalScale;
}
/**
*
* RenderEntity class GetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns cube texture
*
*/
ID3D11ShaderResourceView*
RenderEntity::GetDiffuseMap()
{
	return m_pDiffuseMap;
}
/**
*
* RenderEntity class GetNormalMap
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns cube NormalMap
*
*/
ID3D11ShaderResourceView*
RenderEntity::GetNormalMap()
{
	return m_pNormalMap;
}
/**
*
* RenderEntity class SetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDiffuseMap new model texture
*
*/
void
RenderEntity::SetDiffuseMap( ID3D11ShaderResourceView* _pDiffuseMap )
{
	m_pDiffuseMap = _pDiffuseMap;
}
/**
*
* RenderEntity class Sets the Normal map for this object
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pNormalMap new model normal map
*
*/
void
RenderEntity::SetNormalMap( ID3D11ShaderResourceView* _pNormalMap )
{
	m_pNormalMap = _pNormalMap;
}
/**
*
* RenderEntity class Sends the required texture data to the shader
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pTexture new model texture
*
*/
void
RenderEntity::SendTextureDataToShader( ID3D11DeviceContext* _pDevContext )
{
	if( GetDiffuseMap() )
	{
		ID3D11ShaderResourceView* const textureData[ 2 ] = {GetDiffuseMap(),
															GetNormalMap()};
		_pDevContext->PSSetShaderResources( 0, 2, textureData );
	}
}
/**
*
* RenderEntity class Returns a struct containing vertex data
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _iVertex Vertex index
* @return Returns entity vertex
*
*/
TVertex*
RenderEntity::GetVertexData( int _iVertex )
{
	return &m_pVertices[ _iVertex ];
}
/**
*
* RenderEntity class GetVertexCount
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns entity vertex count
*
*/
int
RenderEntity::GetVertexCount() const
{
	return m_iVertexCount;
}
/**
*
* RenderEntity class SetObjectShader
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pRenderTechnique New rendering technique
*
*/
void
RenderEntity::SetObjectShader( Shader* _pObjectShader )
{
	m_pObjectShader = _pObjectShader;
}
/**
*
* RenderEntity class GetVertexCount
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns rendering technique
*
*/
Shader*
RenderEntity::GetObjectShader()
{
	return m_pObjectShader;
}
/**
*
* RenderEntity class ProcessBillboard
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rBillboardMat Camera inv view matrix
*
*/
void
RenderEntity::ProcessBillboard( Camera* _pCurrentCamera, Math::Matrix& _rBillboardMat )
{
	//Check Direction to camera
	Math::Vector3 vecCamRight;
	vecCamRight = Math::Cross( _pCurrentCamera->GetLook(), Math::Vector3( 0.0f, 1.0f, 0.0f ) );
	if( Math::Dot( vecCamRight, m_vecLook ) > 0.0f )
	{
		m_fBillboardFlip = 1.0f;
	}
	else
	{
		m_fBillboardFlip = -1.0f;
	}

	//Construct billboard matrix
	Math::Matrix scaleMat;
	scaleMat = Math::Scale( Math::MatrixIdentity(), m_vecScale );

	Math::Matrix billboardMat = _rBillboardMat * scaleMat;
	billboardMat.data._41 = m_vecPosition.x;
	billboardMat.data._42 = m_vecPosition.y;
	billboardMat.data._43 = m_vecPosition.z;

	m_matWorld = billboardMat;
}
/**
*
* RenderEntity class IsBillboarded
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not this is billboarded
*
*/
bool
RenderEntity::IsBillboarded() const
{
	return m_bIsBillboarded;
}
/**
*
* RenderEntity class ToggleBillboarded
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bIsBillboard Specifies whether or not this is a billboard
*
*/
void
RenderEntity::ToggleBillboarded( bool _bIsBillboard )
{
	m_bIsBillboarded = _bIsBillboard;
}
/**
*
* RenderEntity class IsShadowed
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not this is included in the shadow functionality
*
*/
bool
RenderEntity::IsShadowed() const
{
	return m_bIsShadowed;
}
/**
*
* RenderEntity class ToggleShadowed
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bIsShadowed Toggles the shadowed state to parameter
*
*/
void
RenderEntity::ToggleShadowed( bool _bIsShadowed )
{
	m_bIsShadowed = _bIsShadowed;
}
/**
*
* RenderEntity class IsTransparent
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not this is included in the standard or transparent render call
*
*/
bool
RenderEntity::IsTransparent() const
{
	return m_bIsTransparent;
}
/**
*
* RenderEntity class ToggleTransparency
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bIsTransparent Toggles the transparency state to parameter
*
*/
void
RenderEntity::ToggleTransparency( bool _bIsTransparent )
{
	m_bIsTransparent = _bIsTransparent;
}
/**
*
* RenderEntity class CalculateBoundingBox
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
RenderEntity::CalculateBoundingBox()
{
	if( m_pVertices )
	{
		m_pBoundingBox = new CBoundingBox();
		m_pBoundingBox->Initialise( m_pVertices, m_iVertexCount );
		m_fRadius = m_pBoundingBox->GetRadius();
	}
}
/**
*
* RenderEntity class GetBoundingBox
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the bounding box for this geometry
*
*/
CBoundingBox*
RenderEntity::GetBoundingBox()
{
	return m_pBoundingBox;
}
/**
*
* RenderEntity class GetBoundingBox
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the bounding box for this geometry
*
*/
float
RenderEntity::GetRadius() const
{
	return m_fRadius;
}
/**
*
* RenderEntity class SetRadius
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fRadius New object radius
*
*/
void
RenderEntity::SetRadius( float _fRadius )
{
	m_fRadius = _fRadius;
}
/**
*
* RenderEntity class HasCollided
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pOtherBox Pointer to other bounding box
* @return Returns true if collision has occurred
*
*/
bool
RenderEntity::HasCollided( RenderEntity* _pOtherEntity )
{
	float fRadius = ( m_fRadius + _pOtherEntity->GetRadius() );
	return ( m_vecPosition - _pOtherEntity->GetPosition() ).LengthSq() < ( fRadius * 0.25f );
}
/**
*
* RenderEntity class DoDraw
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not draw the object
*
*/
bool
RenderEntity::DoDraw() const
{
	return m_bDoDraw;
}
/**
*
* RenderEntity class Sets whether or not to draw the object
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bDoDraw whether or not to draw the object
*
*/
void
RenderEntity::SetDoDraw( bool _bDoDraw )
{
	m_bDoDraw = _bDoDraw;
}