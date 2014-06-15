//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   CRenderEntity.cpp
//  Description :   Code for Class CRenderEntity
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

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
* CRenderEntity class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CRenderEntity::CRenderEntity()
: m_pVertexBuffer(0)
, m_pIndexBuffer(0)
, m_pVertices(0)
, m_pIndices(0)
, m_pObjectShader(0)
, m_pBoundingBox(0)
, m_iVertexCount(0)
, m_iIndexCount(0)
, m_bIsShadowed(false)
, m_bIsTransparent(false)
, m_bIsBillboarded(false)
, m_bDoDraw(true)
, m_fRadius(0.0f)
, m_pDiffuseMap(0)
, m_pNormalMap(0)
, m_pNode(0)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_vecPosition *= 0.0f;
	m_vecScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_vecRotation *= 0.0f;
	
	m_vecLook *= 0.0f;
	m_vecUp *= 0.0f;
	m_vecRight *= 0.0f;

	m_sEntityType = "NONE";
}

/**
*
* CRenderEntity class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CRenderEntity::~CRenderEntity()
{
	if (m_pNode)
	{
		delete m_pNode;
		m_pNode = 0;
	}
	if (m_pBoundingBox)
	{
		delete m_pBoundingBox;
		m_pBoundingBox = 0;
	}
	if (m_pVertices)
	{
		delete[] m_pVertices;
		m_pVertices = 0;
	}
	if (m_pIndices)
	{
		delete[] m_pIndices;
		m_pIndices = 0;
	}
	m_pDiffuseMap = 0;
	m_pNormalMap = 0;
	ReleaseCOM(m_pVertexBuffer);
	ReleaseCOM(m_pIndexBuffer);
}
/**
*
* CRenderEntity class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 device
* @return Returns true
*
*/
bool 
CRenderEntity::Initialise(ID3D11Device* _pDevice, float _fScale)
{
	//Render entity will never be created
	return true;
}
/**
*
* CRenderEntity class Update
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Time elapsed
*
*/
void 
CRenderEntity::Process(float _fDeltaTime, D3DXMATRIX* _pParentMatrix)
{
	if (DoDraw())
	{
		if (m_bIsBillboarded == false)
		{
			//Calculate world matrix
			D3DXQuaternionRotationYawPitchRoll(&m_quatRot, m_vecRotation.y, m_vecRotation.x, m_vecRotation.z);
			D3DXMatrixTransformation(&m_matWorld,
				NULL,
				NULL,
				&m_vecScale,
				NULL,
				&m_quatRot,
				&m_vecPosition);
			//Check if this matrix must be multiplied by the parent matrix
			if (_pParentMatrix)
			{
				m_matWorld = m_matWorld * *_pParentMatrix;
			}

			//Process all children of this entity
			if (m_pNode)
			{
				if (m_pNode->pEntity)
				{
					for (unsigned int iChild = 0; iChild < m_pNode->vecChildren.size(); ++iChild)
					{
						m_pNode->vecChildren[iChild]->pEntity->Process(_fDeltaTime, &m_matWorld);
					}
					//Process all lights attached to this entity
					for (unsigned int iLight = 0; iLight < m_pNode->vecLights.size(); ++iLight)
					{
						CLight* pCurrentLight = m_pNode->vecLights[iLight];
						pCurrentLight->SetPosition(GetPosition() + pCurrentLight->GetOffset());
						pCurrentLight->SetDirection(GetForward());
					}
				}
			}
		}
	}
}
/**
*
* CRenderEntity class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CRenderEntity::Draw(ID3D11DeviceContext* _pDevice)
{
	UINT stride = sizeof(TVertex);
	UINT offset = 0;
	_pDevice->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	_pDevice->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_pDevice->DrawIndexed(m_iIndexCount, 0, 0);
}
/**
*
* CRenderEntity class CreateModelBuffers
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CRenderEntity::CreateVertexBuffer(ID3D11Device* _pDevice)
{
	D3D11_BUFFER_DESC tVertexBufferDesc;
	tVertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tVertexBufferDesc.ByteWidth = sizeof(TVertex)* m_iVertexCount;
	tVertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tVertexBufferDesc.CPUAccessFlags = 0;
	tVertexBufferDesc.MiscFlags = 0;
	tVertexBufferDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA tVertexData;
	tVertexData.pSysMem = m_pVertices;
	tVertexData.SysMemPitch = 0;
	tVertexData.SysMemSlicePitch = 0;

	_pDevice->CreateBuffer(&tVertexBufferDesc, &tVertexData, &m_pVertexBuffer);
}
/**
*
* CRenderEntity class CreateModelBuffers
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CRenderEntity::CreateIndexBuffer(ID3D11Device* _pDevice)
{
	D3D11_BUFFER_DESC tIndexBufferDesc;
	tIndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tIndexBufferDesc.ByteWidth = sizeof(unsigned int) * m_iIndexCount;
	tIndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tIndexBufferDesc.CPUAccessFlags = 0;
	tIndexBufferDesc.MiscFlags = 0;
	tIndexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA tIndexBufferData;
	tIndexBufferData.pSysMem = m_pIndices;
	tIndexBufferData.SysMemPitch = 0;
	tIndexBufferData.SysMemSlicePitch = 0;

	_pDevice->CreateBuffer(&tIndexBufferDesc, &tIndexBufferData, &m_pIndexBuffer);
}
/**
*
* CRenderEntity class Creates an Entity Node for this object
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pParentNode This objects parent
* @return Returns pointer to new node
*
*/
TEntityNode*
CRenderEntity::CreateNode(TEntityNode* _pParentNode)
{
	m_pNode = new TEntityNode(this, _pParentNode);
	if (_pParentNode)
	{
		_pParentNode->vecChildren.push_back(m_pNode);
	}
	else
	{
		//This is the root node
	}
	return m_pNode;
}
/**
*
* CRenderEntity class GetNode Returns this objects "Entity Node"
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns entity node
*
*/
TEntityNode*
CRenderEntity::GetNode() const
{
	return m_pNode;
}
/**
*
* CRenderEntity class GetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns position
*
*/
D3DXVECTOR3&
CRenderEntity::GetPosition()
{
	return m_vecPosition;
}
/**
*
* CRenderEntity class GetRotation
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns rotation
*
*/
D3DXVECTOR3&
CRenderEntity::GetRotation()
{
	return m_vecRotation;
}
/**
*
* CRenderEntity class GetForward
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns Forward
*
*/
D3DXVECTOR3&
CRenderEntity::GetForward()
{
	return m_vecLook;
}
/**
*
* CRenderEntity class GetWorld
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns world matrix
*
*/
D3DXMATRIX&
CRenderEntity::GetWorld()
{
	return m_matWorld;
}
/**
*
* CRenderEntity class GetEntityType
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns type of this entity
*
*/
std::string&
CRenderEntity::GetEntityType()
{
	return m_sEntityType;
}
/**
*
* CRenderEntity class SetRotation
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rVecRotation New rotation vector
*
*/
void
CRenderEntity::SetRotation(D3DXVECTOR3& _rVecRotation)
{
	m_vecRotation = _rVecRotation;
}
/**
*
* CRenderEntity class SetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rVecPosition New position vector
*
*/
void
CRenderEntity::SetPosition(D3DXVECTOR3& _rVecPosition)
{
	m_vecPosition = _rVecPosition;
}
/**
*
* CRenderEntity class SetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rVecPosition New position vector
*
*/
void
CRenderEntity::SetForward(D3DXVECTOR3& _rVecForward)
{
	m_vecRotation.y = atan2(_rVecForward.x, _rVecForward.z);
	m_vecLook = _rVecForward;
}
/**
*
* CRenderEntity class SetPosition
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rVecPosition New position vector
*
*/
void
CRenderEntity::SetEntityType(std::string& _sType)
{
	m_sEntityType = _sType;
}
/**
*
* CRenderEntity class ProcessInpt
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pKeys Pointer to input struct
* @param _fDT Time elapsed
*
*/
bool
CRenderEntity::ProcessInput(TInputStruct& _pKeys, float _fDT)
{
	//Do nothing
	return false;
}
/**
*
* CRenderEntity class SetWorldMatrix
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rWorld New World matrix
*
*/
void
CRenderEntity::SetWorldMatrix(D3DXMATRIX& _rWorld)
{
	m_matWorld = _rWorld;
}
void
CRenderEntity::SetScale(D3DXVECTOR3& _rVecScale)
{
	m_vecScale = _rVecScale;
}
void
CRenderEntity::SetLocalScale(D3DXVECTOR3& _rLocalScale)
{
	m_vecLocalScale = _rLocalScale;
}
D3DXVECTOR3&
CRenderEntity::GetScale()
{
	return m_vecScale;
}
D3DXVECTOR3&
CRenderEntity::GetLocalScale()
{
	return m_vecLocalScale;
}
/**
*
* CRenderEntity class GetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns cube texture
*
*/
ID3D11ShaderResourceView*
CRenderEntity::GetDiffuseMap()
{
	return m_pDiffuseMap;
}
/**
*
* CRenderEntity class GetNormalMap
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns cube NormalMap
*
*/
ID3D11ShaderResourceView*
CRenderEntity::GetNormalMap()
{
	return m_pNormalMap;
}
/**
*
* CRenderEntity class SetTexture
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDiffuseMap new model texture
*
*/
void
CRenderEntity::SetDiffuseMap(ID3D11ShaderResourceView* _pDiffuseMap)
{
	m_pDiffuseMap = _pDiffuseMap;
}
/**
*
* CRenderEntity class Sets the Normal map for this object
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pNormalMap new model normal map
*
*/
void
CRenderEntity::SetNormalMap(ID3D11ShaderResourceView* _pNormalMap)
{
	m_pNormalMap = _pNormalMap;
}
/**
*
* CRenderEntity class Sends the required texture data to the shader
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pTexture new model texture
*
*/
void
CRenderEntity::SendTextureDataToShader(ID3D11DeviceContext* _pDevContext)
{
	if (GetDiffuseMap())
	{
		ID3D11ShaderResourceView* const textureData[2] = {	GetDiffuseMap(),
															GetNormalMap() };
		_pDevContext->PSSetShaderResources(0, 2, textureData);
	}
}
/**
*
* CRenderEntity class Returns a struct containing vertex data
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _iVertex Vertex index
* @return Returns entity vertex
*
*/
TVertex* 
CRenderEntity::GetVertexData(int _iVertex)
{
	return &m_pVertices[_iVertex];
}
/**
*
* CRenderEntity class GetVertexCount
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns entity vertex count
*
*/
int
CRenderEntity::GetVertexCount() const
{
	return m_iVertexCount;
}
/**
*
* CRenderEntity class SetObjectShader
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pRenderTechnique New rendering technique
*
*/
void
CRenderEntity::SetObjectShader(CShader* _pObjectShader)
{
	m_pObjectShader = _pObjectShader;
}
/**
*
* CRenderEntity class GetVertexCount
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns rendering technique
*
*/
CShader*
CRenderEntity::GetObjectShader()
{
	return m_pObjectShader;
}
/**
*
* CRenderEntity class ProcessBillboard
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _rBillboardMat Camera inv view matrix
*
*/
void
CRenderEntity::ProcessBillboard(CCamera* _pCurrentCamera, D3DXMATRIX& _rBillboardMat)
{
	//Check Direction to camera
	D3DXVECTOR3 vecCamRight;
	D3DXVec3Cross(&vecCamRight, &_pCurrentCamera->GetLook(), &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	if (D3DXVec3Dot(&vecCamRight, &m_vecLook) > 0.0f)
	{
		m_fBillboardFlip = 1.0f;
	}
	else
	{
		m_fBillboardFlip = -1.0f;
	}

	//Construct billboard matrix
	D3DXMATRIX scaleMat;
	D3DXMatrixScaling(&scaleMat, m_vecScale.x, m_vecScale.y, m_vecScale.z);

	D3DXMATRIX billboardMat = _rBillboardMat * scaleMat;
	billboardMat._41 = m_vecPosition.x;
	billboardMat._42 = m_vecPosition.y;
	billboardMat._43 = m_vecPosition.z;

	m_matWorld = billboardMat;
}
/**
*
* CRenderEntity class IsBillboarded
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not this is billboarded
*
*/
bool
CRenderEntity::IsBillboarded() const
{
	return m_bIsBillboarded;
}
/**
*
* CRenderEntity class ToggleBillboarded
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bIsBillboard Specifies whether or not this is a billboard
*
*/
void
CRenderEntity::ToggleBillboarded(bool _bIsBillboard)
{
	m_bIsBillboarded = _bIsBillboard;
}
/**
*
* CRenderEntity class IsShadowed
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not this is included in the shadow functionality
*
*/
bool
CRenderEntity::IsShadowed() const
{
	return m_bIsShadowed;
}
/**
*
* CRenderEntity class ToggleShadowed
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bIsShadowed Toggles the shadowed state to parameter
*
*/
void
CRenderEntity::ToggleShadowed(bool _bIsShadowed)
{
	m_bIsShadowed = _bIsShadowed;
}
/**
*
* CRenderEntity class IsTransparent
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not this is included in the standard or transparent render call
*
*/
bool
CRenderEntity::IsTransparent() const
{
	return m_bIsTransparent;
}
/**
*
* CRenderEntity class ToggleTransparency
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bIsTransparent Toggles the transparency state to parameter
*
*/
void
CRenderEntity::ToggleTransparency(bool _bIsTransparent)
{
	m_bIsTransparent = _bIsTransparent;
}
/**
*
* CRenderEntity class CalculateBoundingBox
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CRenderEntity::CalculateBoundingBox()
{
	if (m_pVertices)
	{
		m_pBoundingBox = new CBoundingBox();
		m_pBoundingBox->Initialise(m_pVertices, m_iVertexCount);
		m_fRadius = m_pBoundingBox->GetRadius();
	}
}
/**
*
* CRenderEntity class GetBoundingBox
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the bounding box for this geometry
*
*/
CBoundingBox*
CRenderEntity::GetBoundingBox()
{
	return m_pBoundingBox;
}
/**
*
* CRenderEntity class GetBoundingBox
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the bounding box for this geometry
*
*/
float
CRenderEntity::GetRadius() const
{
	return m_fRadius;
}
/**
*
* CRenderEntity class SetRadius
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fRadius New object radius
*
*/
void
CRenderEntity::SetRadius(float _fRadius)
{
	m_fRadius = _fRadius;
}
/**
*
* CRenderEntity class HasCollided
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pOtherBox Pointer to other bounding box
* @return Returns true if collision has occurred
*
*/
bool
CRenderEntity::HasCollided(CRenderEntity* _pOtherEntity)
{
	float fRadius = (m_fRadius + _pOtherEntity->GetRadius());
	return (D3DXVec3LengthSq(&(m_vecPosition - _pOtherEntity->GetPosition())) < (fRadius * 0.5f));
}
/**
*
* CRenderEntity class DoDraw
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns whether or not draw the object
*
*/
bool
CRenderEntity::DoDraw() const
{
	return m_bDoDraw;
}
/**
*
* CRenderEntity class Sets whether or not to draw the object
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _bDoDraw whether or not to draw the object
*
*/
void
CRenderEntity::SetDoDraw(bool _bDoDraw)
{
	m_bDoDraw = _bDoDraw;
}