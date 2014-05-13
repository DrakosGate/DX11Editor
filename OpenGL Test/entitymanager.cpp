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
#include "prefab.h"

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
: m_pRenderEntities(0)
, m_pTransparentEntities(0)
, m_pCameraDepths(0)
{
	
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
	for (unsigned int iPrefab = 0; iPrefab < m_vecPrefabTypes.size(); ++iPrefab)
	{
		delete m_vecPrefabTypes[iPrefab];
		m_vecPrefabTypes[iPrefab] = 0;
	}
	m_vecPrefabTypes.clear();
	if(m_pRenderEntities)
	{
		for(int iScene = 0; iScene < SCENE_MAX; ++iScene)
		{
			for (unsigned int iObject = 0; iObject < m_pRenderEntities[iScene].size(); ++iObject)
			{
				//delete m_pRenderEntities[iScene][iObject];
				//m_pRenderEntities[iScene][iObject] = 0;
			}
			m_pRenderEntities[iScene].clear();
		}
		delete[] m_pRenderEntities;
		m_pRenderEntities = 0;
	}
	if(m_pTransparentEntities)
	{
		for (int iScene = 0; iScene < SCENE_MAX; ++iScene)
		{
			for (unsigned int iObject = 0; iObject < m_pTransparentEntities[iScene].size(); ++iObject)
			{
				//delete m_pTransparentEntities[iScene][iObject];
				//m_pTransparentEntities[iScene][iObject] = 0;
			}
			m_pTransparentEntities[iScene].clear();
		}
		delete[] m_pTransparentEntities;
		m_pTransparentEntities = 0;
	}
	if(m_pCameraDepths)
	{
		delete[] m_pCameraDepths;
		m_pCameraDepths = 0;
	}
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
	m_pRenderEntities = new std::vector<CRenderEntity*>[SCENE_MAX];
	m_pTransparentEntities = new std::vector<CRenderEntity*>[SCENE_MAX];

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
	for(unsigned int iEntity = 0; iEntity < m_pRenderEntities[_eGameScene].size(); ++iEntity)
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
	
	for(unsigned int iEntity = 0; iEntity < m_pRenderEntities[_eScene].size(); ++iEntity)
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
	if(m_pTransparentEntities[_eScene].size() > 0)
	{
		//Draw entities
		CRenderEntity* pCurrentEntity = 0;
		D3DXMATRIX matBillboard;
		D3DXMatrixInverse(&matBillboard, 0, &_pCurrentCamera->GetViewMatrix());
		//SortTransparentEntities(_pCurrentCamera, _eScene);

		for(unsigned int iEntity = 0; iEntity < m_pTransparentEntities[_eScene].size(); ++iEntity)
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
	for(unsigned int iEntity = 0; iEntity < m_pTransparentEntities[_eGameScene].size(); ++iEntity)
	{
		m_pCameraDepths[iEntity] = D3DXVec3LengthSq(&(m_pTransparentEntities[_eGameScene][iEntity]->GetPosition() - _pCurrentCamera->GetPosition()));
	}

	//Sort camera depths
	bool bIsSorted = false;
	while(bIsSorted == false)
	{
		bIsSorted = true;
		for (unsigned int iEntity = 0; iEntity < m_pTransparentEntities[_eGameScene].size(); ++iEntity)
		{
			for (unsigned int iOther = iEntity + 1; iOther < m_pTransparentEntities[_eGameScene].size(); ++iOther)
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
	if (_pNewEntity->IsTransparent())
	{
		m_pTransparentEntities[_eScene].push_back(_pNewEntity);
	}
	else
	{
		m_pRenderEntities[_eScene].push_back(_pNewEntity);
	}
}
/**
*
* CEntityManager class Adds Prefab to the prefab container
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pNewEntity Entity to add to vector
*
*/
void 
CEntityManager::AddPrefab(TPrefabOptions* _pPrefab)
{
	m_mapPrefabIndex[_pPrefab->pcPrefabName] = m_vecPrefabTypes.size();
	m_vecPrefabTypes.push_back(_pPrefab);
}
/**
*
* CEntityManager class Returns the prefab options
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcPrefabName Name of prefab to return
*
*/
TPrefabOptions*
CEntityManager::GetPrefabOptions(char* _pcPrefabName)
{
	int iPrefabIndex = m_mapPrefabIndex[_pcPrefabName];
	return m_vecPrefabTypes[iPrefabIndex];
}
/**
*
* CEntityManager class Instantiates a prefab from the string specified
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pcPrefabName Name of prefab to instantiate
*
*/
CPrefab*
CEntityManager::InstantiatePrefab(ID3D11Device* _pDevice, char* _pcPrefabName, CShader* _pShader, EGameScene _eScene, D3DXVECTOR3& _rPos, D3DXVECTOR3& _rScale, D3DXVECTOR3& _rRotation, D3DXCOLOR& _rColour)
{
	int ilength = strlen(_pcPrefabName);
	CPrefab* pNewEntity = new CPrefab();
	TPrefabOptions* pPrefabOptions = GetPrefabOptions(_pcPrefabName);

	pNewEntity->Initialise(_pDevice, 1.0f);
	pNewEntity->SetModel(pPrefabOptions->pModel);
	pNewEntity->SetDiffuseMap(pPrefabOptions->pTexture);
	pNewEntity->SetObjectShader(_pShader);
	pNewEntity->SetPosition(_rPos + D3DXVECTOR3(0.0f, _rScale.y * 0.5f, 0.0f)); //Offset the position above the ground
	pNewEntity->SetScale(_rScale);
	pNewEntity->SetRotation(_rRotation);

	AddEntity(pNewEntity, _eScene);
	return pNewEntity;
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