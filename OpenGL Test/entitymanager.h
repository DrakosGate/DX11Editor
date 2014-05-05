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
	void RecreateScene(EGameScene _eScene);
	void EndOfEntityCreation();

	bool IsEntityInFrustum(CCamera* _pCamera, CBoundingBox* _pBoundingBox);

private:
	CEntityManager(const CEntityManager& _krInstanceToCopy);
	const CEntityManager& operator =(const CEntityManager& _krInstanceToCopy);

//Member variables
protected:
	std::vector<TTemporaryEntity*> m_vecTemporaryEntityContainer;
	CRenderEntity*** m_pRenderEntities;
	CRenderEntity*** m_pTransparentEntities;
	int* m_pNumEntities;
	int* m_pNumTransparent;
	float* m_pCameraDepths;
	int m_iTotalTransparentCount;
	
	////Shader variables
	//ID3D11EffectMatrixVariable* m_fxWorldViewProjection;
	//ID3D11EffectMatrixVariable* m_pObjectWorld;
	//
	//ID3D11EffectTechnique* m_pShadowTech;
	//
	//ID3D11EffectShaderResourceVariable* m_pDiffuseMap;
	//ID3D11EffectShaderResourceVariable* m_pNormalMap;
	//ID3D11EffectShaderResourceVariable* m_pSpecularMap;
	//
	//D3DXMATRIX m_matWorldViewProjection;
	//
	////Textures
	//ID3D11ShaderResourceView* m_pSpecularTexture;
	//ID3D11EffectMatrixVariable* m_pTextureMatrix;
	//D3DXMATRIX m_matTexMatrix;
	//
	////Lighting and projection
	//ID3D11EffectMatrixVariable* m_pLightMatrix;
	
};


#endif //ENTITY_MANAGER_H__