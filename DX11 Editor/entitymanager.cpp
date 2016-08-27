
// Library Includes
#include <D3D11.h>

// Local Includes
#include "renderentity.h"
#include "camera.h"
#include "shader.h"
#include "prefab.h"
#include "aihivemind.h"
#include "lightmanager.h"

// This Include
#include "entitymanager.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* EntityManager class Constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
EntityManager::EntityManager()
	: m_pRenderEntities( 0 )
	, m_pTransparentEntities( 0 )
	, m_pCameraDepths( 0 )
{

}
/**
*
* EntityManager class Destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
EntityManager::~EntityManager()
{
	for( unsigned int iPrefab = 0; iPrefab < m_vecPrefabTypes.size(); ++iPrefab )
	{
		delete m_vecPrefabTypes[ iPrefab ];
		m_vecPrefabTypes[ iPrefab ] = 0;
	}
	m_vecPrefabTypes.clear();
	if( m_pRenderEntities )
	{
		for( int iScene = 0; iScene < SCENE_MAX; ++iScene )
		{
			for( unsigned int iObject = 0; iObject < m_pRenderEntities[ iScene ].size(); ++iObject )
			{
				//delete m_pRenderEntities[iScene][iObject];
				//m_pRenderEntities[iScene][iObject] = 0;
			}
			m_pRenderEntities[ iScene ].clear();
		}
		delete[] m_pRenderEntities;
		m_pRenderEntities = 0;
	}
	if( m_pTransparentEntities )
	{
		for( int iScene = 0; iScene < SCENE_MAX; ++iScene )
		{
			for( unsigned int iObject = 0; iObject < m_pTransparentEntities[ iScene ].size(); ++iObject )
			{
				//delete m_pTransparentEntities[iScene][iObject];
				//m_pTransparentEntities[iScene][iObject] = 0;
			}
			m_pTransparentEntities[ iScene ].clear();
		}
		delete[] m_pTransparentEntities;
		m_pTransparentEntities = 0;
	}
	if( m_pCameraDepths )
	{
		delete[] m_pCameraDepths;
		m_pCameraDepths = 0;
	}
}
/**
*
* EntityManager class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns success of initialisation
*
*/
bool
EntityManager::Initialise( ID3D11Device* _pDevice )
{
	//Set resources that will never change
	m_pRenderEntities = new std::vector<RenderEntity*>[ SCENE_MAX ];
	m_pTransparentEntities = new std::vector<RenderEntity*>[ SCENE_MAX ];

	return true;
}
/**
*
* EntityManager class SetLevelInformation Passes necessary level information to the entity manager
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pHivemind AI Hivemind used for adding AI
* @param _pLightManager Light Manager used for adding lights
*
*/
void
EntityManager::SetLevelInformation( AIHiveMind* _pHivemind, LightManager* _pLightManager )
{
	m_pAIHivemind = _pHivemind;
	m_pLightManager = _pLightManager;
}
/**
*
* EntityManager class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void
EntityManager::Process( float _fDeltaTime, EGameScene _eGameScene )
{
	//_pRootNode->pEntity->Process(_fDeltaTime, NULL);
	for( int iScene = 0; iScene < SCENE_MAX; ++iScene )
	{
		for( unsigned int iEntity = 0; iEntity < m_pRenderEntities[ iScene ].size(); ++iEntity )
		{
			if( m_pRenderEntities[ iScene ][ iEntity ]->DoDraw() )
			{
				m_pRenderEntities[ iScene ][ iEntity ]->Process( _fDeltaTime, NULL );
			}
		}
	}
}
/**
*
* EntityManager class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 Device
* @param _pCurrentCamera Pointer to current camera
*
*/
void
EntityManager::Draw( ID3D11DeviceContext* _pDevice, Camera* _pCurrentCamera, EGameScene _eScene )
{
	//Draw entities
	RenderEntity* pCurrentEntity = 0;
	Math::Matrix matBillboard;
	matBillboard = Math::MatrixInverse( nullptr, _pCurrentCamera->GetViewMatrix() );

	for( unsigned int iEntity = 0; iEntity < m_pRenderEntities[ _eScene ].size(); ++iEntity )
	{
		pCurrentEntity = m_pRenderEntities[ _eScene ][ iEntity ];
		if( pCurrentEntity->IsTransparent() )
		{
			//Render this later
		}
		else
		{
			//Check frustum culling of camera
			if( _pCurrentCamera->IsInFrustum( pCurrentEntity->GetPosition(), pCurrentEntity->GetRadius() * pCurrentEntity->GetScale().x ) )
			{
				if( pCurrentEntity->IsBillboarded() )
				{
					pCurrentEntity->ProcessBillboard( _pCurrentCamera, matBillboard );
				}
				//Check if this entity is set to be drawn
				if( pCurrentEntity->DoDraw() && pCurrentEntity )
				{
					//Send object matrices to server
					pCurrentEntity->GetObjectShader()->SendWVPMatrixData( _pDevice, &pCurrentEntity->GetWorld(), &_pCurrentCamera->GetViewMatrix(), &_pCurrentCamera->GetProjection() );

					//Send texture data to shader
					pCurrentEntity->SendTextureDataToShader( _pDevice );

					pCurrentEntity->Draw( _pDevice );
				}
			}
			else
			{
				//This entity is outside of the frustum!
			}
		}
	}
	DrawTransparentEntities( _pDevice, _pCurrentCamera, _eScene );
}
/**
*
* EntityManager class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 Device
* @param _pCurrentCamera Pointer to current camera
*
*/
void
EntityManager::DrawTransparentEntities( ID3D11DeviceContext* _pDevice, Camera* _pCurrentCamera, EGameScene _eScene )
{
	if( m_pTransparentEntities[ _eScene ].size() > 0 )
	{
		//Draw entities
		RenderEntity* pCurrentEntity = 0;
		Math::Matrix matBillboard;
		matBillboard = Math::MatrixInverse( nullptr, _pCurrentCamera->GetViewMatrix() );
		//SortTransparentEntities(_pCurrentCamera, _eScene);

		for( unsigned int iEntity = 0; iEntity < m_pTransparentEntities[ _eScene ].size(); ++iEntity )
		{
			pCurrentEntity = m_pTransparentEntities[ _eScene ][ iEntity ];
			//Check frustum culling of camera
			if( _pCurrentCamera->IsInFrustum( pCurrentEntity->GetPosition(), pCurrentEntity->GetRadius() * pCurrentEntity->GetScale().x ) )
			{
				if( pCurrentEntity->IsBillboarded() )
				{
					pCurrentEntity->ProcessBillboard( _pCurrentCamera, matBillboard );
				}
				//Check if this entity is set to be drawn
				if( pCurrentEntity->DoDraw() && pCurrentEntity )
				{
					//Send object matrices to server
					pCurrentEntity->GetObjectShader()->SendWVPMatrixData( _pDevice, &pCurrentEntity->GetWorld(), &_pCurrentCamera->GetViewMatrix(), &_pCurrentCamera->GetProjection() );

					//Send texture data to shader
					ID3D11ShaderResourceView* const texture[ 1 ] = {pCurrentEntity->GetDiffuseMap()};
					_pDevice->PSSetShaderResources( 0, 1, texture );


					pCurrentEntity->Draw( _pDevice );
				}
			}
			else
			{
				//This entity is outside of the frustum!
			}
		}
	}
}
void
EntityManager::SortTransparentEntities( Camera* _pCurrentCamera, EGameScene _eGameScene )
{
	//Calculate camera depths
	for( unsigned int iEntity = 0; iEntity < m_pTransparentEntities[ _eGameScene ].size(); ++iEntity )
	{
		m_pCameraDepths[ iEntity ] = ( m_pTransparentEntities[ _eGameScene ][ iEntity ]->GetPosition() - _pCurrentCamera->GetPosition() ).LengthSq();
	}

	//Sort camera depths
	bool bIsSorted = false;
	while( bIsSorted == false )
	{
		bIsSorted = true;
		for( unsigned int iEntity = 0; iEntity < m_pTransparentEntities[ _eGameScene ].size(); ++iEntity )
		{
			for( unsigned int iOther = iEntity + 1; iOther < m_pTransparentEntities[ _eGameScene ].size(); ++iOther )
			{
				if( m_pCameraDepths[ iEntity ] < m_pCameraDepths[ iOther ] )
				{
					//Swap entities
					RenderEntity* pSwappedEntity = m_pTransparentEntities[ _eGameScene ][ iEntity ];
					m_pTransparentEntities[ _eGameScene ][ iEntity ] = m_pTransparentEntities[ _eGameScene ][ iOther ];
					m_pTransparentEntities[ _eGameScene ][ iOther ] = pSwappedEntity;
					//Swap camera depths
					float fSwappedDistance = m_pCameraDepths[ iEntity ];
					m_pCameraDepths[ iEntity ] = m_pCameraDepths[ iOther ];
					m_pCameraDepths[ iOther ] = fSwappedDistance;
					bIsSorted = false;
				}
			}
		}
	}
}
/**
*
* EntityManager class DrawProjection
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 Device
* @param _pCurrentCamera Pointer to current camera
* @param _pProjectionCamera Pointer to camera to project from
* @param _pProjectedMap Texture to project
*
*/
void
EntityManager::DrawProjection( ID3D11DeviceContext* _pDevice, Camera* _pCurrentCamera, Camera* _pProjectionCamera, ID3D11ShaderResourceView* _pProjectedMap, EGameScene _eScene )
{
	//m_pLightMatrix->SetMatrix((float*)(_pProjectionCamera->GetViewMatrix() * _pProjectionCamera->GetProjection()));
	//RenderEntity* pCurrentEntity = 0;
	//
	////Set texture resources
	//D3D10_TECHNIQUE_DESC techDesc;
	//m_pShadowTech->GetDesc( &techDesc );	
	//
	////If a projected texture is already defined, use that, otherwise use the model texture
	//if(_pProjectedMap)
	//{
	//	m_pDiffuseMap->SetResource(_pProjectedMap);
	//}
	//for(int iEntity = 0; iEntity < m_pNumEntities[_eScene]; ++iEntity)
	//{
	//	pCurrentEntity = m_pRenderEntities[_eScene][iEntity];
	//	if(pCurrentEntity->IsShadowed())
	//	{
	//		for(UINT p = 0; p < techDesc.Passes; ++p)
	//		{				
	//			//If a projected texture is already defined, use that, otherwise use the model texture
	//			if(_pProjectedMap == 0)
	//			{
	//				m_pDiffuseMap->SetResource(pCurrentEntity->GetTexture());
	//				m_pNormalMap->SetResource(pCurrentEntity->GetNormalMap());
	//			}
	//			m_matWorldViewProjection = pCurrentEntity->GetWorld() * _pCurrentCamera->GetViewMatrix() * _pCurrentCamera->GetProjection();
	//			m_fxWorldViewProjection->SetMatrix((float*)&m_matWorldViewProjection);
	//			m_pObjectWorld->SetMatrix((float*)&pCurrentEntity->GetWorld());
	//			m_pShadowTech->GetPassByIndex( p )->Apply( nullptr );
	//			pCurrentEntity->Draw(_pDevice);
	//		}
	//	}
	//}
}
/**
*
* EntityManager class AddEntity
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pNewEntity Entity to add to vector
*
*/
void
EntityManager::AddEntity( RenderEntity* _pNewEntity, EGameScene _eScene )
{
	if( _pNewEntity->IsTransparent() )
	{
		m_pTransparentEntities[ _eScene ].push_back( _pNewEntity );
	}
	else
	{
		m_pRenderEntities[ _eScene ].push_back( _pNewEntity );
	}
}
/**
*
* EntityManager class Adds Prefab to the prefab container
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pNewEntity Entity to add to vector
*
*/
void
EntityManager::AddPrefab( TPrefabOptions* _pPrefab )
{
	m_mapPrefabIndex[ _pPrefab->pcPrefabName ] = m_vecPrefabTypes.size();
	m_vecPrefabTypes.push_back( _pPrefab );
}
/**
*
* EntityManager class Returns the prefab options
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcPrefabName Name of prefab to return
* @return Returns the prefab description
*
*/
TPrefabOptions*
EntityManager::GetPrefabOptions( std::string& _pcPrefabName )
{
	int iPrefabIndex = m_mapPrefabIndex[ _pcPrefabName ];
	return m_vecPrefabTypes[ iPrefabIndex ];
}
/**
*
* EntityManager class Returns the prefab options
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _iPrefabIndex Index of prefab to return
* @return Returns the prefab description
*
*/
TPrefabOptions*
EntityManager::GetPrefabOptions( int _iPrefabIndex )
{
	return m_vecPrefabTypes[ _iPrefabIndex ];
}
/**
*
* EntityManager class Returns the prefab options
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns the number of prefabs defined
*
*/
int
EntityManager::GetPrefabCount() const
{
	return m_vecPrefabTypes.size();
}
/**
*
* EntityManager class Instantiates a prefab from the string specified
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcPrefabName Name of prefab to instantiate
*
*/
Prefab*
EntityManager::InstantiatePrefab( ID3D11Device* _pDevice,
	TEntityNode* _pParentNode,
	std::string& _pcPrefabName,
	Shader* _pShader,
	std::vector<RenderEntity*>& _rGrassColliders,
	EGameScene _eScene,
	Math::Vector3& _rPos,
	Math::Vector3& _rScale,
	Math::Vector3& _rRotation,
	Math::Colour& _rColour )
{
	Prefab* pNewEntity = new Prefab();
	TPrefabOptions* pPrefabOptions = GetPrefabOptions( _pcPrefabName );
	Math::Vector3 vecInstanceScale = Math::Vector3( _rScale.x * pPrefabOptions->vecScale.x, _rScale.y * pPrefabOptions->vecScale.y, _rScale.z * pPrefabOptions->vecScale.z );

	pNewEntity->Initialise( _pDevice, 1.0f );
	pNewEntity->SetModel( pPrefabOptions->pModel );
	pNewEntity->SetDiffuseMap( pPrefabOptions->pTexture );
	pNewEntity->SetObjectShader( _pShader );
	pNewEntity->SetPosition( _rPos ); //Offset the position above the ground
	pNewEntity->SetScale( vecInstanceScale );
	pNewEntity->SetLocalScale( _rScale );
	pNewEntity->SetRotation( _rRotation );
	pNewEntity->SetEntityType( _pcPrefabName );
	pNewEntity->CreateNode( _pParentNode );

	//Check if this is a static obstacle in the scene
	if( pPrefabOptions->bIsStatic )
	{
		m_pAIHivemind->AddStaticObject( _pDevice, pNewEntity );
	}
	//This object moves
	else
	{
		_rGrassColliders.push_back( pNewEntity );
		//Check if this is controlled by AI
		if( pPrefabOptions->eAIType != AI_INVALID )
		{
			//Only add AI to children of the root node
			if( _pParentNode->pParent == NULL )
			{
				m_pAIHivemind->AddAI( pNewEntity, pPrefabOptions->eAIType );
			}
		}
	}
	AddEntity( pNewEntity, _eScene );

	return pNewEntity;
}
/**
*
* EntityManager class Checks if current entity is in camera frustum
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pCamera Current camera with frustum
* @param _pBoundingBox Object bounding box
*
*/
bool
EntityManager::IsEntityInFrustum( Camera* _pCamera, CBoundingBox* _pBoundingBox )
{
	bool bIsInFrustum = true;

	if( _pBoundingBox )
	{

	}

	return bIsInFrustum;
}
/**
*
* EntityManager class Clears and deletes all objects in the current scene
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _eScene Scene to clear
*
*/
void
EntityManager::ClearScene( EGameScene _eScene )
{
	m_pRenderEntities[ _eScene ].clear();
}