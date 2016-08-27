//
//  File Name   :   RenderEntity.h
//  Description :   Class of RenderEntity
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

// Library Includes
#include <vector>
#include <map>

// Local Includes
#include "datastructures.h"

// Types

// Constants

// Prototypes
struct ID3D11Device;
struct ID3D11DeviceContext;
struct TEntityNode;

class Grass;
class Camera;
class Shader;
class Prefab;
class CBoundingBox;
class AIHiveMind;
class RenderEntity;
class LightManager;

//Structure defined here due to forward declaration
struct TTemporaryEntity
{
	RenderEntity* pEntity;
	EGameScene eScene;
};

class EntityManager
{
//Member functions
public:
	EntityManager();
	virtual ~EntityManager();

	virtual bool Initialise(ID3D11Device* _pDevice);
	virtual void SetLevelInformation(AIHiveMind* _pHivemind, LightManager* _pLightManager);

	virtual void Process(float _fDeltaTime, EGameScene _eGameScene);
	virtual void Draw(ID3D11DeviceContext* _pDevice, Camera* _pCurrentCamera, EGameScene _eScene);
	virtual void DrawTransparentEntities(ID3D11DeviceContext* _pDevice, Camera* _pCurrentCamera, EGameScene _eScene);
	virtual void DrawProjection(ID3D11DeviceContext* _pDevice, Camera* _pCurrentCamera, Camera* _pProjectionCamera, ID3D11ShaderResourceView* _pProjectedMap, EGameScene _eScene);
	void SortTransparentEntities(Camera* _pCurrentCamera, EGameScene _eGameScene);

	void AddEntity(RenderEntity* _pNewEntity, EGameScene _eScene);
	void AddPrefab(TPrefabOptions* _pPrefab);
	TPrefabOptions* GetPrefabOptions(std::string& _pcPrefabName);
	TPrefabOptions* GetPrefabOptions(int _iPrefabIndex);
	int GetPrefabCount() const;

	Prefab* InstantiatePrefab(	ID3D11Device* _pDevice, 
								TEntityNode* _pParentNode, 
								std::string& _pcPrefabName, 
								Shader* _pShader, 
								std::vector<RenderEntity*>& _rGrassColliders,
								EGameScene _eScene, 
								Math::Vector3& _rPos, 
								Math::Vector3& _rScale, 
								Math::Vector3& _rRotation, 
								Math::Colour& _rColour);
	
	bool IsEntityInFrustum(Camera* _pCamera, CBoundingBox* _pBoundingBox);
	void ClearScene(EGameScene _eScene);

private:
	EntityManager(const EntityManager& _krInstanceToCopy);
	const EntityManager& operator =(const EntityManager& _krInstanceToCopy);

//Member variables
protected:
	std::vector<RenderEntity*>* m_pRenderEntities;
	std::vector<RenderEntity*>* m_pTransparentEntities;
	std::vector<TPrefabOptions*> m_vecPrefabTypes;

	std::map<std::string, unsigned int> m_mapPrefabIndex;

	AIHiveMind* m_pAIHivemind;
	LightManager* m_pLightManager;

	float* m_pCameraDepths;
	int m_iTotalTransparentCount;
	
};


#endif //ENTITY_MANAGER_H__