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
enum ERenderTargetType
{
	RENDER_INVALID = -1,
	RENDER_MRT,
	RENDER_DEFERRED,
	RENDER_FINAL,
	RENDER_MAX
};
enum EShaderType
{
	SHADER_INVALID = -1,
	SHADER_POINTSPRITE,		//GS Shaders
	SHADER_LINERENDERER,

	SHADER_OBJECT,			//Object Shaders
	SHADER_UNLITOBJECT,
	SHADER_ANIMOBJECT,

	SHADER_MRT,				//Deferred Rendering Shaders
	SHADER_DEFERRED,

	SHADER_FINALOUTPUT,		//Post processing Shaders
	SHADERPOST_PIXELATE,
	SHADERPOST_RADIALBLUR,
	SHADER_MAX
};
enum EVertexLayoutType
{
	VERTEX_INVALID = -1,
	VERTEX_STATIC,
	VERTEX_ANIMATED,
	VERTEX_POINT,
	VERTEX_MAX
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

class CLevel
{
public:
	CLevel();
	virtual ~CLevel();

	virtual bool Initialise(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext, CDirectXRenderer* _pRenderer, TInputStruct* _pInput, int _iScreenWidth, int _iScreenHeight);
	virtual void CreateEntities(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext);
	virtual void Process(ID3D11Device* _pDevice, float _fDeltaTime);
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
	CResourceManager* m_pResourceManager;
	
	CEntityManager* m_pEntityManager;
	TInputStruct* m_pInput;
	float m_fGameTimeElapsed;

	//Game entities
	//CPlayer* m_pPlayer;
	CPrefab* m_pHuman;
	CPrefab* m_pCreatures;
	CPrefab* m_pTrees;
	CPrefab* m_pCursor;
	CModel* m_pTerrain;
	CModel* m_pSelectionCursor;
	CEditorInterface* m_pEditor;

	//CGrass* m_pGrass;
	CRenderEntity** m_pGrassEntities;
	int m_iNumGrassEntities;
	int m_iNumTrees;
	int m_iNumHumans;
	int m_iNumCreatures;
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