//
//  File Name   :   level.h
//  Description :   Class of Level
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __LEVEL_H__
#define __LEVEL_H__

// Library Includes
#include <d3d11.h>
#include <rapidxml_utils.hpp>

// Local Includes
#include "datastructures.h"

// Types
enum EFontType
{
	FONT_INVALID = -1,
	FONT_DEBUG,
	FONT_SCENEGRAPH,
	FONT_PERFORMANCE,
	FONT_MAX
};
// Constants

// Prototypes
struct ID3D11RasterizerState;
struct ID3D11SamplerState;
struct TEntityNode;
struct TGrassThread;
struct TSceneNode;

class Clock;
class Model;
class CAnimatedModel;
class Grass;
class CScene;
class Prefab;
class AIHiveMind;
class LightManager;
class CPlayer;
class RenderEntity;
class RenderToTexture;
class EntityManager;
class Shader;
class Camera;
class DX11Renderer;
class ResourceManager;
class EditorInterface;
class ThreadPool;
//class OpenCLContext;
class FontRenderer;
class Network;
class PerformanceGraph;
class SceneHierarchy;

class Level
{
public:
	Level();
	virtual ~Level();

	virtual bool Initialise( DX11Renderer* _pRenderer, TSetupStruct* _pSetupData, HWND _hWindow, int _iScreenWidth, int _iScreenHeight );
	virtual void CreateEntities( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext, HWND _hWindow );
	virtual void Process( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, Clock* _pClock, float _fDeltaTime );
	virtual void ProcessEntitySelection( ID3D11Device* _pDevice, float _fDeltaTime );
	virtual void Draw( ID3D11DeviceContext* _pDevice );
	virtual void DrawScene( ID3D11DeviceContext* _pDevice, Shader* _pSceneShader, Camera* _pCurrentCamera, EGameScene _EGameScene );

	virtual void AddTextToSceneGraph( TEntityNode* _pEntityNode, int& _iTextOffset, int _iTabCount );
	virtual void CreateRenderTargets( ID3D11Device* _pDevice );

	void OnResize( int _iWidth, int _iHeight );
	Prefab* CreateObject( ID3D11Device* _pDevice, rapidxml::xml_node<>* _pNode, TEntityNode* _pParentNode );
	Prefab* CreateObject( ID3D11Device* _pDevice, TSceneNode* _pNode, TEntityNode* _pParentNode );
	void LoadLevel( ID3D11Device* _pDevice, char* _pcLevelFilename );
	void SaveLevel( ID3D11Device* _pDevice, char* _pcLevelFilename );
	void AddChildToXMLNode( rapidxml::xml_document<>* _pDocument, rapidxml::xml_node<>* _pParentNode, TEntityNode* _pChildNode );

	void ChangeAIProcessingMethod( EProcessingMethod _eProcessingMethod );
	void ChangeGrassProcessingMethod( EProcessingMethod _eProcessingMethod );

private:
	void LoadShaderData( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext );
	void BuildLevelVertexLayouts( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext );

private:
	EGameScene m_eGameScene;
	ERenderState m_eRenderState;
	EProcessingMethod m_eGrassProcessingMethod;
	EProcessingMethod m_eAIProcessingMethod;
	std::string* m_pcProcessingMethodName;
	TSetupStruct* m_pSetupData;

	DX11Renderer* m_pRenderer;
	ResourceManager* m_pResourceManager;
	SceneHierarchy* m_pSceneHierarchy;
	//OpenCLContext* m_pCLKernel;
	ThreadPool* m_pThreadPool;
	int m_iThreadCount;
	TGrassThread* m_pGrassJobs;

	FontRenderer* m_pFont;
	Network* m_pNetwork;
	PerformanceGraph* m_pGraph;
	int m_iGraphDelay;
	float m_fLogCount;

	EntityManager* m_pEntityManager;
	TEntityNode* m_pRootNode;
	bool m_bInputIsEnabled;
	float m_fGameTimeElapsed;

	//Game entities
	//CPlayer* m_pPlayer;
	Prefab* m_pCursor;
	Model* m_pTerrain;
	Model* m_pSelectionCursor;

	//Editor
	EditorInterface* m_pEditor;
	std::vector<Prefab*> m_pLevelEntities;
	std::string m_sSelectedPrefab;
	bool m_bCreateObject;

	RenderEntity* m_pSelectedObject;
	float m_fSelectedMouseDistance;
	float m_fSelectedObjectScale;
	bool m_bHasSelectedObject;

	Grass* m_pGrass;
	std::vector<RenderEntity*> m_vecGrassEntities;
	EGrassState m_eGrassState;
	float m_fGrassScale;

	AIHiveMind* m_pHivemind;

	Camera* m_pCamera;
	Camera* m_pOrthoCamera;

	//Lighting
	LightManager* m_pLightManager;

	//Render targets
	RenderToTexture* m_pRenderTargets;
	RenderToTexture* m_pDiffuseMRT;
	RenderToTexture* m_pNormalsMRT;
	RenderToTexture* m_pPositionMRT;
	RenderToTexture* m_pDepthMRT;
	ID3D11RenderTargetView** m_pMRT;

	bool bLastMouseState;

	Model* m_pRenderTarget;	//Main screen render target
	Model* m_pRenderMonitor;	//Smaller monitors to render to

	Shader* m_pShaderCollection;

	//Temp Matrices
	Math::Matrix m_matWorld;
	Math::Matrix m_matProjection;
	Math::Matrix m_matWorldViewProjection;

	//Vertex Information
	ID3D11InputLayout** m_pVertexLayout;
	ID3D11RasterizerState* m_pRasteriserState;
	ID3D11RasterizerState* m_pGrassRasteriser;
	ID3D11SamplerState* m_pSamplerState;

	//Screen info
	int m_iScreenWidth;
	int m_iScreenHeight;
	float m_pClearColour[ 4 ];
};


#endif //__DIRECTXAPP_H__