//
//  File Name   :   level.h
//  Description :   Class of CLevel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __LEVEL_H__
#define __LEVEL_H__

// Library Includes

// Local Includes
#include "defines.h"

// Types
struct TUCHARColour
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};
// Constants

// Prototypes
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

class CModel;
class CAnimatedModel;
class CGrass;
class CScene;
class CPrefab;
class CAIHiveMind;
class CLightManager;
class CPlayer;
class CRenderToTexture;
class CRenderEntity;
class CEntityManager;
class CShader;
class CCamera;
class CDirectXRenderer;
class CResourceManager;
class CEditorInterface;
class CThreadPool;
class COpenCLKernel;
class CFontRenderer;

class CLevel
{
public:
	CLevel();
	virtual ~CLevel();

	virtual bool Initialise(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext, CDirectXRenderer* _pRenderer, TInputStruct* _pInput, int _iScreenWidth, int _iScreenHeight);
	virtual void CreateEntities(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext);
	virtual void Process(ID3D11Device* _pDevice, float _fDeltaTime);
	virtual bool ProcessInput(ID3D11Device* _pDevice, float _fDeltaTime);
	virtual void Draw(ID3D11DeviceContext* _pDevice);
	virtual void DrawScene(ID3D11DeviceContext* _pDevice, CCamera* _pCurrentCamera, EGameScene _EGameScene);
	
	virtual void CreateRenderTargets(ID3D11Device* _pDevice);
	
	void OnResize(int _iWidth, int _iHeight);	

private:
	void LoadShaderData(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext);
	void BuildLevelVertexLayouts(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext);
 
private:
	CDirectXRenderer* m_pRenderer;
	EGameScene m_eGameScene;
	ERenderState m_eRenderState;
	CResourceManager* m_pResourceManager;
	CThreadPool* m_pThreadPool;
	COpenCLKernel* m_pOpenCLKernel;
	CFontRenderer* m_pFont;

	CEntityManager* m_pEntityManager;
	TInputStruct* m_pInput;
	float m_fGameTimeElapsed;

	//Game entities
	//CPlayer* m_pPlayer;
	std::vector<CPrefab*> m_pHumans;
	std::vector<CPrefab*> m_pCreatures;
	std::vector<CPrefab*> m_pTrees;
	CPrefab* m_pCursor;
	CModel* m_pTerrain;
	CModel* m_pSelectionCursor;

	//Editor
	CEditorInterface* m_pEditor;
	std::vector<CPrefab*> m_pNewEntities;
	std::string m_sSelectedPrefab;
	bool m_bCreateObject;

	CRenderEntity* m_pSelectedObject;
	bool m_bHasSelectedObject;

	//CGrass* m_pGrass;
	CRenderEntity** m_pGrassEntities;
	int m_iNumGrassEntities;
	float m_fGrassScale;

	CAIHiveMind* m_pHivemind;
	
	CCamera* m_pCamera;
	CCamera* m_pOrthoCamera;
	
	//Lighting
	CLightManager* m_pLightManager;
	
	//Render targets
	CRenderToTexture* m_pRenderTargets;
	CRenderToTexture* m_pDiffuseMRT;
	CRenderToTexture* m_pNormalsMRT;
	CRenderToTexture* m_pPositionMRT;
	CRenderToTexture* m_pDepthMRT;
	ID3D11RenderTargetView** m_pMRT;
	
	bool bLastMouseState;
	
	CModel* m_pRenderTarget;	//Main screen render target
	CModel* m_pRenderMonitor;	//Smaller monitors to render to

	CShader* m_pShaderCollection;
	
	//Temp Matrices
	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matProjection;
	D3DXMATRIX m_matWorldViewProjection;
	
	//Vertex Information
	ID3D11InputLayout** m_pVertexLayout;
	ID3D11RasterizerState* m_pRasteriserState;
	ID3D11SamplerState* m_pSamplerState;

	//Screen info
	int m_iScreenWidth;
	int m_iScreenHeight;
	float m_pClearColour[4];
};


#endif //__DIRECTXAPP_H__