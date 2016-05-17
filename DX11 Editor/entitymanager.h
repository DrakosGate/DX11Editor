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
#include "defines.h"

// Types

// Constants

// Prototypes
struct ID3D11Device;
struct ID3D11DeviceContext;
struct TEntityNode;

class CGrass;
class Camera;
class CShader;
class CPrefab;
class CBoundingBox;
class CAIHiveMind;
class RenderEntity;
class CLightManager;

//Structure defined here due to forward declaration
struct TTemporaryEntity
{
	RenderEntity* pEntity;
	EGameScene eScene;
};

class CEntityManager
{
//Member functions
public:
	CEntityManager();
	virtual ~CEntityManager();

	virtual bool Initialise(ID3D11Device* _pDevice);
	virtual void SetLevelInformation(CAIHiveMind* _pHivemind, CLightManager* _pLightManager);

	virtual void Process(float _fDeltaTime, EGameScene _eGameScene);
	virtual void Draw(ID3D11DeviceContext* _pDevice, Camera* _pCurrentCamera, EGameScene _eScene);
	virtual void DrawTransparentEntities(ID3D11DeviceContext* _pDevice, Camera* _pCurrentCamera, EGameScene _eScene);
	virtual void DrawProjection(ID3D11DeviceContext* _pDevice, Camera* _pCurrentCamera, Camera* _pProjectionCamera, ID3D10ShaderResourceView* _pProjectedMap, EGameScene _eScene);
	void SortTransparentEntities(Camera* _pCurrentCamera, EGameScene _eGameScene);

	void AddEntity(RenderEntity* _pNewEntity, EGameScene _eScene);
	void AddPrefab(TPrefabOptions* _pPrefab);
	TPrefabOptions* GetPrefabOptions(std::string& _pcPrefabName);
	TPrefabOptions* GetPrefabOptions(int _iPrefabIndex);
	int GetPrefabCount() const;

	CPrefab* InstantiatePrefab(	ID3D11Device* _pDevice, 
								TEntityNode* _pParentNode, 
								std::string& _pcPrefabName, 
								CShader* _pShader, 
								std::vector<RenderEntity*>& _rGrassColliders,
								EGameScene _eScene, 
								Math::Vector3& _rPos, 
								Math::Vector3& _rScale, 
								Math::Vector3& _rRotation, 
								Math::Colour& _rColour);
	
	bool IsEntityInFrustum(Camera* _pCamera, CBoundingBox* _pBoundingBox);
	void ClearScene(EGameScene _eScene);

private:
	CEntityManager(const CEntityManager& _krInstanceToCopy);
	const CEntityManager& operator =(const CEntityManager& _krInstanceToCopy);

//Member variables
protected:
	std::vector<RenderEntity*>* m_pRenderEntities;
	std::vector<RenderEntity*>* m_pTransparentEntities;
	std::vector<TPrefabOptions*> m_vecPrefabTypes;

	std::map<std::string, unsigned int> m_mapPrefabIndex;

	CAIHiveMind* m_pAIHivemind;
	CLightManager* m_pLightManager;

	float* m_pCameraDepths;
	int m_iTotalTransparentCount;
	
};


#endif //ENTITY_MANAGER_H__