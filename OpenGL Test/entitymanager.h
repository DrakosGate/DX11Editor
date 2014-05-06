//
//  File Name   :   CRenderEntity.h
//  Description :   Class of CRenderEntity
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

// Library Includes
#include <vector>

// Local Includes
#include "defines.h"

// Types

// Constants

// Prototypes
struct ID3D11Device;
struct ID3D11DeviceContext;

class CRenderEntity;
class CCamera;
class CShader;
class CPrefab;
class CBoundingBox;

//Structure defined here due to forward declaration
struct TTemporaryEntity
{
	CRenderEntity* pEntity;
	EGameScene eScene;
};

class CEntityManager
{
//Member functions
public:
	CEntityManager();
	virtual ~CEntityManager();

	virtual bool Initialise(ID3D11Device* _pDevice);
	virtual void Process(float _fDeltaTime, EGameScene _eGameScene, CCamera* _pCurrentCamera);
	virtual void Draw(ID3D11DeviceContext* _pDevice, CCamera* _pCurrentCamera, EGameScene _eScene);
	virtual void DrawTransparentEntities(ID3D11DeviceContext* _pDevice, CCamera* _pCurrentCamera, EGameScene _eScene);
	virtual void DrawProjection(ID3D11DeviceContext* _pDevice, CCamera* _pCurrentCamera, CCamera* _pProjectionCamera, ID3D10ShaderResourceView* _pProjectedMap, EGameScene _eScene);
	void SortTransparentEntities(CCamera* _pCurrentCamera, EGameScene _eGameScene);

	void AddEntity(CRenderEntity* _pNewEntity, EGameScene _eScene);
	void AddPrefab(TPrefabOptions* _pPrefab);
	TPrefabOptions* GetPrefabOptions(char* _pcPrefabName);
	CPrefab* InstantiatePrefab(ID3D11Device* _pDevice, char* _pcPrefabName, CShader* _pShader, EGameScene _eScene, D3DXVECTOR3& _rPos, D3DXVECTOR3& _rScale, D3DXVECTOR3& _rRotation, D3DXCOLOR& _rColour);
	
	bool IsEntityInFrustum(CCamera* _pCamera, CBoundingBox* _pBoundingBox);

private:
	CEntityManager(const CEntityManager& _krInstanceToCopy);
	const CEntityManager& operator =(const CEntityManager& _krInstanceToCopy);

//Member variables
protected:
	std::vector<CRenderEntity*>* m_pRenderEntities;
	std::vector<CRenderEntity*>* m_pTransparentEntities;

	std::vector<TPrefabOptions*> m_vecPrefabTypes;

	float* m_pCameraDepths;
	int m_iTotalTransparentCount;
	
};


#endif //ENTITY_MANAGER_H__