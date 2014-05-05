//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   entitymanager.cpp
//  Description :   Code for Class CEntityManager
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <D3D11.h>

// Local Includes
#include "renderentity.h"
#include "camera.h"
#include "shader.h"

// This Include
#include "entitymanager.h"

// Static Variables

// Static Function Prototypes

// Implementation

/**
*
* CEntityManager class Constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CEntityManager::CEntityManager()
:	m_pRenderEntities(0)
//,	m_fxWorldViewProjection(0)
,	m_pNumEntities(0)
//,	m_pDiffuseMap(0)
//,	m_pSpecularMap(0)
//,	m_pSpecularTexture(0)
//,	m_pTextureMatrix(0)
,	m_pCameraDepths(0)
{
	//D3DXMatrixIdentity(&m_matTexMatrix);
}
/**
*
* CEntityManager class Destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CEntityManager::~CEntityManager()
{
	if(m_pRenderEntities)
	{
		for(int iScene = 0; iScene < SCENE_MAX; ++iScene)
		{
			delete[] m_pRenderEntities[iScene];
			m_pRenderEntities[iScene] = 0;
		}
		delete[] m_pRenderEntities;
		m_pRenderEntities = 0;
	}
	if(m_pTransparentEntities)
	{
		for(int iScene = 0; iScene < SCENE_MAX; ++iScene)
		{
			delete[] m_pTransparentEntities[iScene];
			m_pTransparentEntities[iScene] = 0;
		}
		delete[] m_pTransparentEntities;
		m_pTransparentEntities = 0;
	}
	if(m_pCameraDepths)
	{
		delete[] m_pCameraDepths;
		m_pCameraDepths = 0;
	}
	if(m_pNumEntities)
	{
		delete[] m_pNumEntities;
		m_pNumEntities = 0;
	}
	if(m_pNumTransparent)
	{
		delete[] m_pNumTransparent;
		m_pNumTransparent = 0;
	}
	m_vecTemporaryEntityContainer.clear();
}
/**
*
* CEntityManager class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns success of initialisation
*
*/
bool 
CEntityManager::Initialise(ID3D11Device* _pDevice)
{
	//Set resources that will never change
	
	return true;
}
/**
*
* CEntityManager class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void 
CEntityManager::Process(float _fDeltaTime, EGameScene _eGameScene, CCamera* _pCurrentCamera)
{
	for(int iEntity = 0; iEntity < m_pNumEntities[_eGameScene]; ++iEntity)
	{
		if(m_pRenderEntities[_eGameScene][iEntity]->DoDraw())
		{
			m_pRenderEntities[_eGameScene][iEntity]->Process(_fDeltaTime);
		}
	}
}
/**
*
* CEntityManager class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 Device
* @param _pCurrentCamera Pointer to current camera
*
*/
void 
CEntityManager::Draw(ID3D11DeviceContext* _pDevice, CCamera* _pCurrentCamera, EGameScene _eScene)
{
	//Draw entities
	CRenderEntity* pCurrentEntity = 0;
	D3DXMATRIX matBillboard;
	D3DXMatrixInverse(&matBillboard, 0, &_pCurrentCamera->GetViewMatrix());
	
	for(int iEntity = 0; iEntity < m_pNumEntities[_eScene]; ++iEntity)
	{
		pCurrentEntity = m_pRenderEntities[_eScene][iEntity];
		if(pCurrentEntity->IsTransparent())
		{
			//Render this later
		}
		else
		{
			//Check frustum culling of camera
			if(_pCurrentCamera->IsInFrustum(pCurrentEntity->GetPosition(), pCurrentEntity->GetRadius()))
			{
				if(pCurrentEntity->IsBillboarded())
				{
					pCurrentEntity->ProcessBillboard(_pCurrentCamera, matBillboard);
				}
				//Check if this entity is set to be drawn
				if(pCurrentEntity->DoDraw() && pCurrentEntity)
				{
					//Send object matrices to server
					pCurrentEntity->GetObjectShader()->SendWVPMatrixData(_pDevice, &pCurrentEntity->GetWorld(), &_pCurrentCamera->GetViewMatrix(), &_pCurrentCamera->GetProjection());

					//Send texture data to shader
					pCurrentEntity->SendTextureDataToShader(_pDevice);

					pCurrentEntity->Draw(_pDevice);
				}
			}
			else
			{
				//This entity is outside of the frustum!
			}
		}
	}
	DrawTransparentEntities(_pDevice, _pCurrentCamera, _eScene);
}
/**
*
* CEntityManager class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pDevice DirectX 10 Device
* @param _pCurrentCamera Pointer to current camera
*
*/
void 
CEntityManager::DrawTransparentEntities(ID3D11DeviceContext* _pDevice, CCamera* _pCurrentCamera, EGameScene _eScene)
{
	if(m_pNumTransparent[_eScene] > 0)
	{
		//Draw entities
		CRenderEntity* pCurrentEntity = 0;
		D3DXMATRIX matBillboard;
		D3DXMatrixInverse(&matBillboard, 0, &_pCurrentCamera->GetViewMatrix());
		//SortTransparentEntities(_pCurrentCamera, _eScene);

		for(int iEntity = 0; iEntity < m_pNumTransparent[_eScene]; ++iEntity)
		{
			pCurrentEntity = m_pTransparentEntities[_eScene][iEntity];
			//Check frustum culling of camera
			if(_pCurrentCamera->IsInFrustum(pCurrentEntity->GetPosition(), pCurrentEntity->GetRadius()))
			{
				if(pCurrentEntity->IsBillboarded())
				{
					pCurrentEntity->ProcessBillboard(_pCurrentCamera, matBillboard);
				}
				//Check if this entity is set to be drawn
				if(pCurrentEntity->DoDraw() && pCurrentEntity)
				{
					//Send object matrices to server
					pCurrentEntity->GetObjectShader()->SendWVPMatrixData(_pDevice, &pCurrentEntity->GetWorld(), &_pCurrentCamera->GetViewMatrix(), &_pCurrentCamera->GetProjection());
					
					//Send texture data to shader
					ID3D11ShaderResourceView* const texture[1] = { pCurrentEntity->GetDiffuseMap() };
					_pDevice->PSSetShaderResources(0, 1, texture);


					pCurrentEntity->Draw(_pDevice);
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
CEntityManager::SortTransparentEntities(CCamera* _pCurrentCamera, EGameScene _eGameScene)
{
	//Calculate camera depths
	for(int iEntity = 0; iEntity < m_pNumTransparent[_eGameScene]; ++iEntity)
	{
		m_pCameraDepths[iEntity] = D3DXVec3LengthSq(&(m_pTransparentEntities[_eGameScene][iEntity]->GetPosition() - _pCurrentCamera->GetPosition()));
	}

	//Sort camera depths
	bool bIsSorted = false;
	while(bIsSorted == false)
	{
		bIsSorted = true;
		for(int iEntity = 0; iEntity < m_pNumTransparent[_eGameScene]; ++iEntity)
		{
			for(int iOther = iEntity + 1; iOther < m_pNumTransparent[_eGameScene]; ++iOther)
			{
				if(m_pCameraDepths[iEntity] < m_pCameraDepths[iOther])
				{
					//Swap entities
					CRenderEntity* pSwappedEntity = m_pTransparentEntities[_eGameScene][iEntity];
					m_pTransparentEntities[_eGameScene][iEntity] = m_pTransparentEntities[_eGameScene][iOther];
					m_pTransparentEntities[_eGameScene][iOther] = pSwappedEntity;
					//Swap camera depths
					float fSwappedDistance = m_pCameraDepths[iEntity];
					m_pCameraDepths[iEntity] = m_pCameraDepths[iOther];
					m_pCameraDepths[iOther] = fSwappedDistance;
					bIsSorted = false;
				}
			}
		}
	}
}
/**
*
* CEntityManager class DrawProjection
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
CEntityManager::DrawProjection(ID3D11DeviceContext* _pDevice, CCamera* _pCurrentCamera, CCamera* _pProjectionCamera, ID3D10ShaderResourceView* _pProjectedMap, EGameScene _eScene)
{
	//m_pLightMatrix->SetMatrix((float*)(_pProjectionCamera->GetViewMatrix() * _pProjectionCamera->GetProjection()));
	//CRenderEntity* pCurrentEntity = 0;
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
	//			m_pShadowTech->GetPassByIndex( p )->Apply(0);
	//			pCurrentEntity->Draw(_pDevice);
	//		}
	//	}
	//}
}
/**
*
* CEntityManager class AddEntity
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pNewEntity Entity to add to vector
*
*/
void 
CEntityManager::AddEntity(CRenderEntity* _pNewEntity, EGameScene _eScene)
{
	//Add this entity to the temporary vector
	TTemporaryEntity* tTemp = new TTemporaryEntity;
	tTemp->pEntity = _pNewEntity;
	tTemp->eScene = _eScene;
	m_vecTemporaryEntityContainer.push_back(tTemp);
}
/**
*
* CEntityManager class Transfers data from temporary vector to a scene
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _eScene Scene to recreate
*
*/
void 
CEntityManager::RecreateScene(EGameScene _eScene)
{
	//Delete this scene if it already exists
	if(m_pRenderEntities[_eScene])
	{
		delete[] m_pRenderEntities[_eScene];
		m_pRenderEntities[_eScene] = 0;
	}
	//Count number of entities waiting to be added to this scene
	int iNumEntities = 0;
	for(unsigned int iEntity = 0; iEntity < m_vecTemporaryEntityContainer.size(); ++iEntity)
	{
		if(m_vecTemporaryEntityContainer[iEntity]->eScene == _eScene)
		{
			++iNumEntities;
		}
	}
	m_pNumEntities[_eScene] = iNumEntities;
	//Create new array of entities and transfer vector data
	m_pRenderEntities[_eScene] = new CRenderEntity*[iNumEntities];
	for(unsigned int iEntity = 0; iEntity < m_vecTemporaryEntityContainer.size(); ++iEntity)
	{
		//WARNING!!! This will assume the vector ONLY contains entities for one scene
		m_pRenderEntities[_eScene][iEntity] = m_vecTemporaryEntityContainer[iEntity]->pEntity;
		delete m_vecTemporaryEntityContainer[iEntity];
	}
	m_vecTemporaryEntityContainer.clear();
}
/**
*
* CEntityManager class EndOfEntityCreation Specifies the last object has been added
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CEntityManager::EndOfEntityCreation()
{
	int iTotalEntities = m_vecTemporaryEntityContainer.size();
	m_pNumEntities = new int[SCENE_MAX];
	m_pNumTransparent = new int[SCENE_MAX];
	for(int i = 0; i < SCENE_MAX; ++i)
	{
		m_pNumEntities[i] = 0;
		m_pNumTransparent[i] = 0;
	}
	//Count type of each entity
	for(int i = 0; i < iTotalEntities; ++i)
	{
		//Increment the number of entities per this scene
		if(m_vecTemporaryEntityContainer[i]->pEntity->IsTransparent())
		{
			++m_pNumTransparent[m_vecTemporaryEntityContainer[i]->eScene];
		}
		else
		{
			++m_pNumEntities[m_vecTemporaryEntityContainer[i]->eScene];
		}
	}

	//Create the array of render entities
	m_pRenderEntities = new CRenderEntity**[SCENE_MAX];
	m_pTransparentEntities = new CRenderEntity**[SCENE_MAX];
	int* pCurrentEntity = new int[SCENE_MAX];
	int* pCurrentTransparent = new int[SCENE_MAX];
	for(int i = 0; i < SCENE_MAX; ++i)
	{
		pCurrentEntity[i] = 0;
		pCurrentTransparent[i] = 0;
		m_pRenderEntities[i] = new CRenderEntity*[m_pNumEntities[i]];
		m_pTransparentEntities[i] = new CRenderEntity*[m_pNumTransparent[i]];
	}
	//Transfer entities from vector to the new array
	m_iTotalTransparentCount = 0;
	for(int iEntity = 0; iEntity < iTotalEntities; ++iEntity)
	{
		//				[SCENE]  [Entity Index]
		TTemporaryEntity* pEntity = m_vecTemporaryEntityContainer[iEntity];
		if(pEntity->pEntity->IsTransparent())
		{
			m_pTransparentEntities[pEntity->eScene][pCurrentTransparent[pEntity->eScene]] = pEntity->pEntity;
			++pCurrentTransparent[pEntity->eScene];
			++m_iTotalTransparentCount;
		}
		else
		{
			m_pRenderEntities[pEntity->eScene][pCurrentEntity[pEntity->eScene]] = pEntity->pEntity;
			++pCurrentEntity[pEntity->eScene];
		}
	}
	m_pCameraDepths = new float[m_iTotalTransparentCount];

	//Clear out the old vector container
	delete[] pCurrentEntity;
	delete[] pCurrentTransparent;
	pCurrentEntity = 0;
	pCurrentTransparent = 0;

	for(int iEntity = 0; iEntity < iTotalEntities; ++iEntity)
	{
		delete m_vecTemporaryEntityContainer[iEntity];
		m_vecTemporaryEntityContainer[iEntity] = 0;
	}
	m_vecTemporaryEntityContainer.clear();
}
/**
*
* CEntityManager class Checks if current entity is in camera frustum
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pCamera Current camera with frustum
* @param _pBoundingBox Object bounding box
*
*/
bool 
CEntityManager::IsEntityInFrustum(CCamera* _pCamera, CBoundingBox* _pBoundingBox)
{
	bool bIsInFrustum = true;

	if(_pBoundingBox)
	{

	}

	return bIsInFrustum;
}