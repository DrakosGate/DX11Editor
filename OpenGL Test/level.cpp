//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   level.cpp
//  Description :   Code for CLevel
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <D3D11.h>

// Local Includes
#include "defines.h"
#include "entitymanager.h"
#include "model.h"
#include "animatedmodel.h"
//#include "grass.h"
#include "boundingbox.h"
#include "prefab.h"
#include "aihivemind.h"
#include "lightmanager.h"
#include "pointlight.h"
#include "spotlight.h"
#include "directionallight.h"
//#include "player.h"
#include "shader.h"
#include "camera.h"
#include "rendertotexture.h"
#include "directxrenderer.h"
#include "audioplayer.h"
#include "resourcemanager.h"
#include "editorinterface.h"

// This Include
#include "level.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CLevel class constructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CLevel::CLevel()
: m_pInput(0)
, m_pRenderer(0)
, m_eGameScene(SCENE_3DSCENE)
, m_pEntityManager(0)
//, m_pPlayer(0)
, m_pHuman(0)
, m_pCreatures(0)
, m_pTrees(0)
, m_pCursor(0)
, m_pTerrain(0)
//, m_pGrass(0)
, m_pGrassEntities(0)
, m_pHivemind(0)
, m_pCamera(0)
, m_pOrthoCamera(0)
, m_pLightManager(0)
, m_pRenderTargets(0)
, m_pDiffuseMRT(0)
, m_pNormalsMRT(0)
, m_pPositionMRT(0)
, m_pMRT(0)
, m_pRenderTarget(0)
, m_pRenderMonitor(0)
, m_pShaderCollection(0)
, m_pVertexLayout(0)
, m_pSelectionCursor(0)
, m_fGrassScale(0.0f)
, m_fGameTimeElapsed(0.0f)
, m_iScreenWidth(0)
, m_iScreenHeight(0)
, m_iNumGrassEntities(0)
, m_iNumTrees(0)
, m_iNumHumans(0)
, m_iNumCreatures(0)
, m_pResourceManager(0)
{
	D3DXMatrixIdentity(&m_matWorldViewProjection); 
}
/**
*
* CLevel class destructor
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
CLevel::~CLevel()
{
	//Cleanup singletons
	CAudioPlayer::GetInstance().DestroyInstance();
	//if(m_pPlayer)
	//{
	//	delete m_pPlayer;
	//	m_pPlayer = 0;
	//}
	if (m_pEditor)
	{
		delete m_pEditor;
		m_pEditor = 0;
	}
	if(m_pCursor)
	{
		delete m_pSelectionCursor;
		m_pSelectionCursor = 0;
		delete m_pCursor;
		m_pCursor = 0;
	}
	if(m_pGrassEntities)
	{
		delete[] m_pGrassEntities;
		m_pGrassEntities = 0;
	}
	if(m_pLightManager)
	{
		delete m_pLightManager;
		m_pLightManager = 0;
	}
	if(m_pHivemind)
	{
		delete m_pHivemind;
		m_pHivemind = 0;
	}
	if(m_pTrees)
	{
		for (int iTree = 0; iTree < m_iNumTrees; ++iTree)
		{
			delete m_pTrees[iTree];
			m_pTrees[iTree] = 0;
		}
		delete[] m_pTrees;
		m_pTrees = 0;
	}
	if(m_pCreatures)
	{
		for (int iCreature = 0; iCreature < m_iNumCreatures; ++iCreature)
		{
			delete m_pCreatures[iCreature];
			m_pCreatures[iCreature] = 0;
		}
		delete[] m_pCreatures;
		m_pCreatures = 0;
	}
	if(m_pHuman)
	{
		for (int iHuman = 0; iHuman < m_iNumHumans; ++iHuman)
		{
			delete m_pHuman[iHuman];
			m_pHuman[iHuman] = 0;
		}
		delete[] m_pHuman;
		m_pHuman = 0;
	}
	if(m_pTerrain)
	{
		delete m_pTerrain;
		m_pTerrain = 0;
	}
	//if(m_pGrass)
	//{
	//	delete m_pGrass;
	//	m_pGrass = 0;
	//}
	if(m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = 0;

		delete m_pOrthoCamera;
		m_pOrthoCamera = 0;
	}
	//Clear render targets
	if(m_pDiffuseMRT)
	{
		delete m_pDiffuseMRT;
		m_pDiffuseMRT = 0;
	}
	if(m_pNormalsMRT)
	{
		delete m_pNormalsMRT;
		m_pNormalsMRT = 0;
	}
	if (m_pPositionMRT)
	{
		delete m_pPositionMRT;
		m_pPositionMRT = 0;
	}
	if(m_pMRT)
	{
		delete[] m_pMRT;
		m_pMRT = 0;
	}
	if(m_pRenderTargets)
	{
		delete[] m_pRenderTargets;
		m_pRenderTargets = 0;
	}

	if(m_pRenderMonitor)
	{
		delete[] m_pRenderMonitor;
		m_pRenderMonitor = 0;
	}
	if(m_pRenderTarget)
	{
		delete m_pRenderTarget;
		m_pRenderTarget = 0;
	}
	if (m_pEntityManager)
	{
		delete m_pEntityManager;
		m_pEntityManager = 0;
	}
	if (m_pResourceManager)
	{
		delete m_pResourceManager;
		m_pResourceManager = 0;
	}

	//Clear shaders
	if(m_pShaderCollection)
	{
		delete[] m_pShaderCollection;
		m_pShaderCollection = 0;
	}
	ReleaseCOM(m_pRasteriserState);
	ReleaseCOM(m_pSamplerState);
	for (int iLayout = 0; iLayout < VERTEX_MAX; ++iLayout)
	{
		ReleaseCOM(m_pVertexLayout[iLayout]);
	}
	delete[] m_pVertexLayout;
	m_pVertexLayout = 0;
}
/**
*
* CLevel class Initialise
* (Task ID: n/a)
*
* @author Christopher Howlett
* @return Returns initialisation success
*
*/
bool
CLevel::Initialise(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext, CDirectXRenderer* _pRenderer, TInputStruct* _pInput, int _iScreenWidth, int _iScreenHeight)
{
	m_pRenderer = _pRenderer;
	m_pInput = _pInput;
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;

	m_pClearColour[0] = 0.01f;
	m_pClearColour[1] = 0.01f;
	m_pClearColour[2] = 0.1f;
	m_pClearColour[3] = 1.0f;

	LoadShaderData(_pDevice, _pDevContext);
	BuildLevelVertexLayouts(_pDevice, _pDevContext);

	//Create render targets and screen monitors
	CreateRenderTargets(_pDevice);
	
	//Create renderable entities and add to entitymanager
	CreateEntities(_pDevice, _pDevContext);

	float fAspectRatio = static_cast<float>(m_iScreenWidth) / static_cast<float>(m_iScreenHeight);
	m_pCamera = new CCamera();
	m_pCamera->Initialise(20.0f, 0.1f, 10.0f, m_iScreenWidth, m_iScreenHeight, true);
	m_pCamera->SetPosition(D3DXVECTOR3(0.0f, 2.0f, -15.0f));
	D3DXVECTOR3 vecCameraLook;
	D3DXVec3Normalize(&vecCameraLook, &(D3DXVECTOR3(0.0f, 0.0f, 0.0f) - m_pCamera->GetPosition()));
	m_pCamera->SetLook(vecCameraLook);
	m_pCamera->CreateProjectionMatrix(fAspectRatio);

	m_pOrthoCamera = new CCamera();
	m_pOrthoCamera->Initialise(0.0f, 0.0f, 0.0f, static_cast<int>(m_iScreenWidth), static_cast<int>(m_iScreenHeight), false);
	m_pOrthoCamera->SetPosition(D3DXVECTOR3(0.0f, 0.0f, -1000.0f));
	D3DXVec3Normalize(&vecCameraLook, &(D3DXVECTOR3(0.0f, 0.0f, 0.0f) - m_pOrthoCamera->GetPosition()));
	m_pOrthoCamera->SetLook(vecCameraLook);
	m_pOrthoCamera->CreateProjectionMatrix(fAspectRatio);

	//m_pPlayer = new CPlayer();
	//m_pPlayer->Initialise(m_pInput, m_pCamera, m_pCursor);
	
	//Initialise lighting
	m_pLightManager = new CLightManager();
	m_pLightManager->Initialise();
	
	//Directional
	//m_pLightManager->AddDirectional(D3DXVECTOR3(0.0f, -0.2f, 1.0f), D3DXCOLOR(0.5f, 0.6f, 0.5f, 1.0f), 5.0f);
	//Point
	for(int i = 0; i < m_iNumHumans; ++i)
	{
		m_pLightManager->AddPoint(D3DXVECTOR3(0.0f, 0.4f, 0.0f), D3DXCOLOR(0.7f, 0.7f, 0.3f, 1.0f), D3DXVECTOR3(0.15f, 0.02f, 5.0f), 1.0f);
	}
	//Cursor light
	m_pLightManager->AddPoint(D3DXVECTOR3(0.0f, 0.4f, 0.0f), D3DXCOLOR(0.1f, 0.1f, 0.5f, 1.0f), D3DXVECTOR3(0.05f, 0.2f, 4.0f), 1.0f);
	//Spot
	//m_pLightManager->AddSpot(D3DXVECTOR3(0.0f, 15.0f, .0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR3(1.0f, 0.5f, 0.02f), 1.5f, 5000.0f);

	CAudioPlayer::GetInstance().Initialise(false);
	CAudioPlayer::GetInstance().Play3DSound(SOUND_BIRDCHIRP, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	float fFieldOfView = static_cast<float>(D3DX_PI)* 0.25f;
	float fScreenAspect = static_cast<float>(_iScreenWidth) / static_cast<float>(_iScreenHeight);
	D3DXMatrixPerspectiveFovLH(&m_matProjection, fFieldOfView, fScreenAspect, 0.1f, 1000.0f);
	D3DXMatrixIdentity(&m_matWorld);


	return true;
}
/**
*
* CDirectXRenderer class CreateEntities
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CLevel::CreateEntities(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext)
{	
	printf("================= Creating Entities =================\n");
	//Create entity manager to contain all objects
	m_pEntityManager = new CEntityManager();
	m_pEntityManager->Initialise(_pDevice);

	//Read level resources
	m_pResourceManager = new CResourceManager();
	m_pResourceManager->Initialise(_pDevice, "Data/Resources.xml");
	m_pResourceManager->LoadPrefabTypes(_pDevice, m_pEntityManager, "Data/Prefabs.xml");

	m_pRenderTarget = new CModel();
	m_pRenderTarget->Initialise();
	m_pRenderTarget->LoadSquare(_pDevice, 1.0f, D3DXVECTOR2(1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pRenderTarget->SetObjectShader(&m_pShaderCollection[SHADER_FINALOUTPUT]);
	m_pRenderTarget->SetScale(D3DXVECTOR3(WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f));
	m_pEntityManager->AddEntity(m_pRenderTarget, SCENE_FINAL);
	
	m_pTerrain = new CModel();
	m_pTerrain->Initialise();
	m_pTerrain->LoadSquare(_pDevice, 100.0f, D3DXVECTOR2(100.0f, 100.0f), D3DXCOLOR(1.0f, 0.9f, 1.0f, 1.0f));
	m_pTerrain->SetRotation(D3DXVECTOR3((static_cast<float>(D3DX_PI) * 0.5f), 0.0f, 0.0f));
	m_pTerrain->SetObjectShader(&m_pShaderCollection[SHADER_MRT]);
	m_pTerrain->SetDiffuseMap(m_pResourceManager->GetTexture("grass"));
	m_pEntityManager->AddEntity(m_pTerrain, SCENE_3DSCENE);
	m_pTerrain->SetRadius(FLT_MAX);

	//m_fGrassScale = 15.0f;
	//m_pGrass = new CGrass();
	//m_pGrass->Initialise();
	//m_pGrass->LoadTerrain(_pDevice, 100, 100, m_fGrassScale, D3DXVECTOR2(10.0f, 10.0f));
	//m_pGrass->SetRenderTechnique(m_pObjectShader->GetEffect()->GetTechniqueByName("GrassMRT"));
	//m_pGrass->SetDiffuseMap(m_pResourceManager->GetTexture("grassblades"));
	//m_pGrass->SetRadius(FLT_MAX);
	//m_pEntityManager->AddEntity(m_pGrass, SCENE_GRASS);
	
	m_pCursor = new CPrefab();
	m_pCursor->Initialise(_pDevice, 1.0f);
	m_pCursor->SetModel(m_pResourceManager->GetModel("cursor"));
	m_pCursor->SetDiffuseMap(m_pResourceManager->GetTexture("cursor"));
	m_pCursor->SetObjectShader(&m_pShaderCollection[SHADER_MRT]);
	m_pCursor->SetPosition(D3DXVECTOR3(0.0f, 0.4f, 0.0f));
	m_pCursor->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	m_pCursor->SetRotation(D3DXVECTOR3(0.0f, -0.5f, 0.0f));
	m_pEntityManager->AddEntity(m_pCursor, SCENE_3DSCENE);

	//Setup AI
	m_pHivemind = new CAIHiveMind();
	m_pHivemind->Initialise();
	m_pHivemind->CreateNavigationGrid(_pDevice, m_pEntityManager, &m_pShaderCollection[SHADER_LINERENDERER], 10.0f, 40, 40);

	m_iNumHumans = 5;
	m_iNumCreatures = 20;
	m_iNumTrees = 20;
	m_pHuman = new CPrefab*[m_iNumHumans];
	//Add all entities to the grass avoidance
	m_iNumGrassEntities = 1 + m_iNumCreatures + m_iNumHumans; // + 1 for the cursor
	m_pGrassEntities = new CRenderEntity*[m_iNumGrassEntities];
	int iCurrentGrassEntity = 0;
	m_pGrassEntities[iCurrentGrassEntity] = m_pCursor;
	++iCurrentGrassEntity;

	for(int iHuman = 0; iHuman < m_iNumHumans; ++iHuman)
	{
		//m_pHuman[iHuman].Initialise(_pDevice, 1.0f);
		//m_pHuman[iHuman].SetModel(m_pResourceManager->GetModel("human"));
		//m_pHuman[iHuman].SetDiffuseMap(m_pResourceManager->GetTexture("human"));
		//m_pHuman[iHuman].SetObjectShader(&m_pShaderCollection[SHADER_MRT]);
		//m_pHuman[iHuman].SetPosition(D3DXVECTOR3(0.0f, 0.5f, 0.0f));
		m_pHuman[iHuman] = m_pEntityManager->InstantiatePrefab(	_pDevice,
																"human",
																&m_pShaderCollection[SHADER_MRT],
																SCENE_3DSCENE,
																D3DXVECTOR3(D3DXVECTOR3(0.0f, 0.5f, 0.0f)),
																D3DXVECTOR3(1.0f, 1.0f, 1.0f),
																D3DXVECTOR3(0.0f, static_cast<float>(rand() % 360), 0.0f),
																D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pHivemind->AddAI(m_pHuman[iHuman], AI_HUMAN);
		//m_pEntityManager->AddEntity(m_pHuman[iHuman], SCENE_3DSCENE);

		m_pGrassEntities[iCurrentGrassEntity] = m_pHuman[iHuman];
		++iCurrentGrassEntity;
	}

	m_pCreatures = new CPrefab*[m_iNumCreatures];
	for(int iCreature = 0; iCreature < m_iNumCreatures; ++iCreature)
	{
		//m_pCreatures[iCreature].Initialise(_pDevice, 1.0f);
		//m_pCreatures[iCreature].SetModel(m_pResourceManager->GetModel("chicken"));
		//m_pCreatures[iCreature].SetDiffuseMap(m_pResourceManager->GetTexture("chicken"));
		//m_pCreatures[iCreature].SetObjectShader(&m_pShaderCollection[SHADER_ANIMOBJECT]);
		//m_pCreatures[iCreature].SetPosition(D3DXVECTOR3(0.0f, 0.25f, 0.0f));
		//m_pCreatures[iCreature].SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
		m_pCreatures[iCreature] = m_pEntityManager->InstantiatePrefab(	_pDevice,
																		"chicken",
																		&m_pShaderCollection[SHADER_MRT],
																		SCENE_3DSCENE,
																		D3DXVECTOR3(D3DXVECTOR3(0.0f, 0.25f, 0.0f)),
																		D3DXVECTOR3(0.5f, 0.5f, 0.5f),
																		D3DXVECTOR3(0.0f, static_cast<float>(rand() % 360), 0.0f),
																		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pHivemind->AddAI(m_pCreatures[iCreature], AI_CREATURE);
		//m_pEntityManager->AddEntity(&m_pCreatures[iCreature], SCENE_3DSCENE);
		//m_pEntityManager->AddEntity(&m_pCreatures[iCreature], SCENE_3DANIM);

		m_pGrassEntities[iCurrentGrassEntity] = m_pCreatures[iCreature];
		++iCurrentGrassEntity;
	}

	m_pTrees = new CPrefab*[m_iNumTrees];
	float fTreeRadius = 4.0f;
	float fTreeDensity = 0.1f;
	for(int iTree = 0; iTree < m_iNumTrees; ++iTree)
	{
		float fCurrentTreeRadius = fTreeRadius + (iTree * fTreeDensity);
		m_pTrees[iTree] = m_pEntityManager->InstantiatePrefab(	_pDevice,
																"tree",
																&m_pShaderCollection[SHADER_MRT],
																SCENE_3DSCENE,
																D3DXVECTOR3(sinf(static_cast<float>(iTree)) * fCurrentTreeRadius, 0.75f, cosf(static_cast<float>(iTree)) * fCurrentTreeRadius),
																D3DXVECTOR3(1.5f, 1.5f, 1.5f),
																D3DXVECTOR3(0.0f, static_cast<float>(rand() % 360), 0.0f),
																D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pHivemind->AddStaticObject(_pDevice, m_pTrees[iTree]);
	}
	m_pSelectionCursor = new CModel();
	m_pSelectionCursor->Initialise();
	m_pSelectionCursor->LoadSquare(_pDevice, 1.0f, D3DXVECTOR2(1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pSelectionCursor->SetPosition(D3DXVECTOR3(0.0f, 0.4f, 0.0f));
	m_pSelectionCursor->SetObjectShader(&m_pShaderCollection[SHADER_MRT]);
	m_pSelectionCursor->SetDiffuseMap(m_pResourceManager->GetTexture("selectionbox"));
	//m_pEntityManager->AddEntity(m_pSelectionCursor, SCENE_3DSCENE);

	m_pEditor = new CEditorInterface();
	m_pEditor->Initialise();
	m_pEditor->LoadFromXML(_pDevice, m_pResourceManager, "Data/EditorLayout.xml");
	m_pEditor->SetObjectShader(&m_pShaderCollection[SHADER_POINTSPRITE]);
	m_pEditor->SetDiffuseMap(m_pResourceManager->GetTexture("menu_button"));
	m_pEntityManager->AddEntity(m_pEditor, SCENE_UI);

	_pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_pDevContext->IASetInputLayout(m_pVertexLayout[VERTEX_STATIC]);
	_pDevContext->PSSetSamplers(0, 1, &m_pSamplerState);
}
/**
*
* CLevel class Process
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
void 
CLevel::Process(ID3D11Device* _pDevice, float _fDeltaTime)
{
	m_pCamera->ProcessInput(m_pInput, D3DXVECTOR2(1.0f, 0.0f), _fDeltaTime);
	m_pCamera->Process(_fDeltaTime);
	m_pOrthoCamera->Process(_fDeltaTime);

	m_pInput->m_tMouseRay = m_pCamera->GetMouseRay(m_pInput->vecMouse);
	float fIntersectionPoint = PlaneToLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_pInput->m_tMouseRay);
	if (fIntersectionPoint > 0.0f)
	{
		m_pCursor->SetPosition(m_pInput->m_tMouseRay.vecPosition + (m_pInput->m_tMouseRay.vecDirection * fIntersectionPoint));
		float fCameraRotation = (static_cast<float>(D3DX_PI) * 0.9f) + atan2f(m_pCamera->GetLook().x, m_pCamera->GetLook().z);
		m_pCursor->SetRotation(D3DXVECTOR3(0.0f, fCameraRotation, 0.0f));
	}

	for (int i = 0; i < m_iNumHumans; ++i)
	{
		m_pLightManager->GetPoint(i)->SetPosition(m_pHuman[i]->GetPosition() + m_pHuman[i]->GetForward() * 0.5f);
	}
	m_pLightManager->GetPoint(m_iNumHumans)->SetPosition(m_pCursor->GetPosition() + D3DXVECTOR3(0.0f, 0.1f, 0.0f));
	
	//Process camera and movement
	//m_pPlayer->ProcessInput(_fDeltaTime);
	m_pEditor->ProcessInput(*m_pInput, _fDeltaTime);
	m_pEditor->RefreshBuffers(_pDevice);
	m_pEditor->Process(_fDeltaTime);
	
	//Process entities
	m_pHivemind->Process(_fDeltaTime);
	//m_pResourceManager->GetAnimatedModel("chicken")->Process(_fDeltaTime);
	m_pEntityManager->Process(_fDeltaTime, SCENE_3DSCENE, m_pCamera);
	m_pEntityManager->Process(_fDeltaTime, SCENE_3DANIM, m_pCamera);
	m_pEntityManager->Process(_fDeltaTime, SCENE_UI, m_pCamera);
	m_pEntityManager->Process(_fDeltaTime, SCENE_DEBUG, m_pCamera);
	m_pEntityManager->Process(_fDeltaTime, SCENE_FINAL, m_pCamera);
	m_fGameTimeElapsed += _fDeltaTime;
	
	//float fGrassOffset = m_fGrassScale * 0.1f;
	//m_pGrass->RecreateGrassMesh(_pDevice, 
	//							m_pCursor->GetPosition() + (m_pCamera->GetLook() * fGrassOffset), 
	//							m_pGrassEntities, 
	//							m_iNumGrassEntities,
	//							_fDeltaTime);
	//m_pInput->fMouseWheel = 0.0f;

	//Process audio
	CAudioPlayer::GetInstance().SetListenerPosition(m_pCamera->GetPosition(), m_pCamera->GetLook(), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	CAudioPlayer::GetInstance().Process();
}
/*
*
* CLevel class Draw
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CLevel::Draw(ID3D11DeviceContext* _pDevice)
{
	//Send lighting info to shader
	m_pShaderCollection[SHADER_OBJECT].SendLightInformation(_pDevice, m_pLightManager, m_pCamera);

	_pDevice->IASetInputLayout(m_pVertexLayout[VERTEX_STATIC]);
	_pDevice->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//============================ Multiple Render Texture ================================
	//Set shader for rendering
	
	_pDevice->OMSetDepthStencilState(m_pRenderTargets[RENDER_MRT].GetAddDepthState(), 0);
	_pDevice->OMSetRenderTargets(3, m_pMRT, m_pDiffuseMRT->GetDepthStencilView());

	m_pDiffuseMRT->ClearRenderTarget(_pDevice, 0, m_pClearColour);
	m_pNormalsMRT->ClearRenderTarget(_pDevice, 0, m_pClearColour);
	m_pPositionMRT->ClearRenderTarget(_pDevice, 0, m_pClearColour);
	
	// Render to MRT

	//=== DRAW ANIMATED CHARACTERS ===
	_pDevice->VSSetShader(m_pShaderCollection[SHADER_ANIMOBJECT].GetVertexShader(), NULL, 0);
	_pDevice->GSSetShader(NULL, NULL, 0);
	_pDevice->PSSetShader(m_pShaderCollection[SHADER_ANIMOBJECT].GetPixelShader(), NULL, 0);
	_pDevice->IASetInputLayout(m_pVertexLayout[VERTEX_ANIMATED]);
	DrawScene(_pDevice, m_pCamera, SCENE_3DANIM);

	//=== DRAW STILL CHARACTERS ===
	_pDevice->VSSetShader(m_pShaderCollection[SHADER_MRT].GetVertexShader(), NULL, 0);
	_pDevice->GSSetShader(NULL, NULL, 0);
	_pDevice->PSSetShader(m_pShaderCollection[SHADER_MRT].GetPixelShader(), NULL, 0);
	_pDevice->IASetInputLayout(m_pVertexLayout[VERTEX_STATIC]);
	DrawScene(_pDevice, m_pCamera, SCENE_3DSCENE);

	//=== DRAW GRASS ===
	//_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
	//DrawScene(_pDevice, pActiveCamera, SCENE_GRASS);
	//_pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	//============================ Deferred Render ================================
	_pDevice->VSSetShader(m_pShaderCollection[SHADER_DEFERRED].GetVertexShader(), NULL, 0);
	_pDevice->GSSetShader(NULL, NULL, 0);
	_pDevice->PSSetShader(m_pShaderCollection[SHADER_DEFERRED].GetPixelShader(), NULL, 0);
	m_pRenderTargets[RENDER_DEFERRED].SetRenderTarget(_pDevice, 1, m_pNormalsMRT->GetDepthStencilView());
	m_pRenderTargets[RENDER_DEFERRED].ClearRenderTarget(_pDevice, 0, m_pClearColour);

	//Send MRT texture data to shader
	ID3D11ShaderResourceView* const texture[3] = {	m_pDiffuseMRT->GetRenderShaderResource(),
													m_pNormalsMRT->GetRenderShaderResource(), 
													m_pPositionMRT->GetRenderShaderResource() };
	_pDevice->PSSetShaderResources(0, 3, texture);
	

	m_pRenderTarget->SetDiffuseMap(m_pDiffuseMRT->GetRenderShaderResource());
	DrawScene(_pDevice, m_pOrthoCamera, SCENE_FINAL);
	m_pRenderTarget->SetDiffuseMap(m_pRenderTargets[RENDER_DEFERRED].GetRenderShaderResource());
	
	//============================ FINAL PASS ================================
	m_pRenderer->PrepareLastScene();
	
	//Prepare swapchain buffers
	_pDevice->VSSetShader(m_pShaderCollection[SHADER_FINALOUTPUT].GetVertexShader(), NULL, 0);
	_pDevice->GSSetShader(NULL, NULL, 0);
	_pDevice->PSSetShader(m_pShaderCollection[SHADER_FINALOUTPUT].GetPixelShader(), NULL, 0);
	DrawScene(_pDevice, m_pOrthoCamera, SCENE_FINAL);	

	//Render debug lines to screen
	_pDevice->IASetInputLayout(m_pVertexLayout[VERTEX_POINT]);
	_pDevice->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	_pDevice->VSSetShader(m_pShaderCollection[SHADER_LINERENDERER].GetVertexShader(), NULL, 0);
	_pDevice->GSSetShader(m_pShaderCollection[SHADER_LINERENDERER].GetGeometryShader(), NULL, 0);
	_pDevice->PSSetShader(m_pShaderCollection[SHADER_LINERENDERER].GetPixelShader(), NULL, 0);
	DrawScene(_pDevice, m_pCamera, SCENE_DEBUG);

	//Render UI objects to screen
	_pDevice->VSSetShader(m_pShaderCollection[SHADER_POINTSPRITE].GetVertexShader(), NULL, 0);
	_pDevice->GSSetShader(m_pShaderCollection[SHADER_POINTSPRITE].GetGeometryShader(), NULL, 0);
	_pDevice->PSSetShader(m_pShaderCollection[SHADER_POINTSPRITE].GetPixelShader(), NULL, 0);
	m_pResourceManager->SendTextureDataToShader(_pDevice);
	DrawScene(_pDevice, m_pOrthoCamera, SCENE_UI);

	//Unbind all render targets and shader resources prior to next frame
	ID3D11ShaderResourceView* const blankTexture[3] = { NULL, NULL, NULL };
	_pDevice->PSSetShaderResources(0, 3, blankTexture);
	_pDevice->OMSetRenderTargets(0, 0, NULL);
}
/**
*
* CLevel class DrawScene Draws the entire scene from perspective on given camera
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _pCurrentCamera Camera to render from
*
*/
void CLevel::DrawScene(ID3D11DeviceContext* _pDevice, CCamera* _pCurrentCamera, EGameScene _EGameScene)
{
	//Draw all entities in scene
	m_pEntityManager->Draw(_pDevice, _pCurrentCamera, _EGameScene);
}
/**
*
* CLevel class CreateRenderTargets
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void 
CLevel::CreateRenderTargets(ID3D11Device* _pDevice)
{
	m_pRenderTargets = new CRenderToTexture[RENDER_MAX];
	m_pRenderTargets[RENDER_MRT].Initialise(_pDevice, true, true, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pRenderTargets[RENDER_DEFERRED].Initialise(_pDevice, true, true, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pRenderTargets[RENDER_FINAL].Initialise(_pDevice, true, true, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	
	m_pDiffuseMRT = new CRenderToTexture();
	m_pDiffuseMRT->Initialise(_pDevice, true, true, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pNormalsMRT = new CRenderToTexture();
	m_pNormalsMRT->Initialise(_pDevice, true, true, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pPositionMRT = new CRenderToTexture();
	m_pPositionMRT->Initialise(_pDevice, true, true, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pMRT = new ID3D11RenderTargetView*[3];
	m_pMRT[0] = m_pDiffuseMRT->GetRenderTarget();
	m_pMRT[1] = m_pNormalsMRT->GetRenderTarget();
	m_pMRT[2] = m_pPositionMRT->GetRenderTarget();
}
/**
*
* CLevel class LoadShaderData
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CLevel::LoadShaderData(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext)
{
	m_pShaderCollection = new CShader[SHADER_MAX];
	m_pShaderCollection[SHADER_POINTSPRITE].Initialise(_pDevice);
	m_pShaderCollection[SHADER_POINTSPRITE].CompileVertexShader(_pDevice, L"Shaders/pointsprite_vs.hlsl", "PointVS");
	m_pShaderCollection[SHADER_POINTSPRITE].CompileGeometryShader(_pDevice, L"Shaders/pointsprite_gs.hlsl", "PointGS");
	m_pShaderCollection[SHADER_POINTSPRITE].CompilePixelShader(_pDevice, L"Shaders/pointsprite_ps.hlsl", "PointPS");

	m_pShaderCollection[SHADER_LINERENDERER].Initialise(_pDevice);
	m_pShaderCollection[SHADER_LINERENDERER].CompileVertexShader(_pDevice, L"Shaders/linerenderer_vs.hlsl", "LineVS");
	m_pShaderCollection[SHADER_LINERENDERER].CompileGeometryShader(_pDevice, L"Shaders/linerenderer_gs.hlsl", "LineGS");
	m_pShaderCollection[SHADER_LINERENDERER].CompilePixelShader(_pDevice, L"Shaders/linerenderer_ps.hlsl", "LinePS");

	m_pShaderCollection[SHADER_OBJECT].Initialise(_pDevice);
	m_pShaderCollection[SHADER_OBJECT].CompileVertexShader(_pDevice, L"Shaders/objectshader_vs.hlsl", "ObjectVS");
	m_pShaderCollection[SHADER_OBJECT].CompilePixelShader(_pDevice, L"Shaders/objectshader_ps.hlsl", "ObjectPS");

	m_pShaderCollection[SHADER_UNLITOBJECT].Initialise(_pDevice);
	m_pShaderCollection[SHADER_UNLITOBJECT].CompileVertexShader(_pDevice, L"Shaders/objectshader_vs.hlsl", "ObjectVS");
	m_pShaderCollection[SHADER_UNLITOBJECT].CompilePixelShader(_pDevice, L"Shaders/objectshader_ps.hlsl", "UnlitObjectPS");

	m_pShaderCollection[SHADER_ANIMOBJECT].Initialise(_pDevice);
	m_pShaderCollection[SHADER_ANIMOBJECT].CompileVertexShader(_pDevice, L"Shaders/animatedobject_vs.hlsl", "AnimVS");
	m_pShaderCollection[SHADER_ANIMOBJECT].CompilePixelShader(_pDevice, L"Shaders/objectshader_ps.hlsl", "ObjectPS");

	m_pShaderCollection[SHADER_MRT].Initialise(_pDevice);
	m_pShaderCollection[SHADER_MRT].CompileVertexShader(_pDevice, L"Shaders/objectshader_vs.hlsl", "ObjectVS");
	m_pShaderCollection[SHADER_MRT].CompilePixelShader(_pDevice, L"Shaders/mrtshader_ps.hlsl", "MRTPS");

	m_pShaderCollection[SHADER_DEFERRED].Initialise(_pDevice);
	m_pShaderCollection[SHADER_DEFERRED].CompileVertexShader(_pDevice, L"Shaders/objectshader_vs.hlsl", "ObjectVS");
	m_pShaderCollection[SHADER_DEFERRED].CompilePixelShader(_pDevice, L"Shaders/deferred_ps.hlsl", "DeferredPS");

	m_pShaderCollection[SHADER_FINALOUTPUT].Initialise(_pDevice);
	m_pShaderCollection[SHADER_FINALOUTPUT].CompileVertexShader(_pDevice, L"Shaders/objectshader_vs.hlsl", "ObjectVS");
	m_pShaderCollection[SHADER_FINALOUTPUT].CompilePixelShader(_pDevice, L"Shaders/postprocessing.hlsl", "FinalColourPS");

	m_pShaderCollection[SHADERPOST_PIXELATE].Initialise(_pDevice);
	m_pShaderCollection[SHADERPOST_PIXELATE].CompileVertexShader(_pDevice, L"Shaders/objectshader_vs.hlsl", "ObjectVS");
	m_pShaderCollection[SHADERPOST_PIXELATE].CompilePixelShader(_pDevice, L"Shaders/postprocessing.hlsl", "PixelatePS");

	m_pShaderCollection[SHADERPOST_RADIALBLUR].Initialise(_pDevice);
	m_pShaderCollection[SHADERPOST_RADIALBLUR].CompileVertexShader(_pDevice, L"Shaders/objectshader_vs.hlsl", "ObjectVS");
	m_pShaderCollection[SHADERPOST_RADIALBLUR].CompilePixelShader(_pDevice, L"Shaders/postprocessing.hlsl", "RadialBlurPS");
}
/**
*
* CLevel class buildVertexLayouts
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CLevel::BuildLevelVertexLayouts(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext)
{
	m_pVertexLayout = new ID3D11InputLayout*[VERTEX_MAX];
	for (int iLayout = 0; iLayout < VERTEX_MAX; ++iLayout)
	{
		m_pVertexLayout[iLayout] = 0;
	}

	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	int iNumElements = 6;

	// Create the input layout
	HRCheck(_pDevice->CreateInputLayout(	vertexDesc, 
											iNumElements, 
											m_pShaderCollection[SHADER_OBJECT].GetShaderBlob()->GetBufferPointer(), 
											m_pShaderCollection[SHADER_OBJECT].GetShaderBlob()->GetBufferSize(), 
											&m_pVertexLayout[VERTEX_STATIC]),
											L"Could not create static input layout");
	// Create the animated vertex input layout.
	D3D11_INPUT_ELEMENT_DESC animDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEID", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	iNumElements = 8;

	// Create the input layout
	HRCheck(_pDevice->CreateInputLayout(animDesc,
										iNumElements,
										m_pShaderCollection[SHADER_ANIMOBJECT].GetShaderBlob()->GetBufferPointer(),
										m_pShaderCollection[SHADER_ANIMOBJECT].GetShaderBlob()->GetBufferSize(),
										&m_pVertexLayout[VERTEX_ANIMATED]),
										L"Could not create animated input layout");

	D3D11_INPUT_ELEMENT_DESC pointDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "DIRECTION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SCALE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXID", 0, DXGI_FORMAT_R32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	iNumElements = 6;

	// Create the input layout
	HRCheck(_pDevice->CreateInputLayout(pointDesc,
										iNumElements,
										m_pShaderCollection[SHADER_POINTSPRITE].GetShaderBlob()->GetBufferPointer(),
										m_pShaderCollection[SHADER_POINTSPRITE].GetShaderBlob()->GetBufferSize(),
										&m_pVertexLayout[VERTEX_POINT]),
										L"Could not create point sprite input layout");

	D3D11_RASTERIZER_DESC tRasterDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	//=================  CREATE RASTERISER STATE ====================
	tRasterDesc.AntialiasedLineEnable = false;
	tRasterDesc.CullMode = D3D11_CULL_BACK;
	tRasterDesc.DepthBias = 0;
	tRasterDesc.DepthBiasClamp = 0.0f;
	tRasterDesc.DepthClipEnable = true;
	tRasterDesc.FillMode = D3D11_FILL_SOLID;
	tRasterDesc.FrontCounterClockwise = false;
	tRasterDesc.ScissorEnable = false;
	tRasterDesc.SlopeScaledDepthBias = 0.0f;
	HRCheck(_pDevice->CreateRasterizerState(&tRasterDesc, &m_pRasteriserState),
		L"Could not create rasteriser state");
	_pDevContext->RSSetState(m_pRasteriserState);

	//=================  CREATE TEXTURE SAMPLER STATE ====================
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRCheck(_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState),
		L"Failed to create sampler state");
}
/**
*
* CLevel class onResize Resizes the window and buffers
* (Task ID: n/a)
*
* @author Christopher Howlett
*
*/
void
CLevel::OnResize(int _iWidth, int _iHeight)
{
	float fAspectRatio = static_cast<float>(_iWidth)/static_cast<float>(_iHeight);
	if(m_pCamera)
	{
		m_pCamera->CreateProjectionMatrix(fAspectRatio);
		m_pOrthoCamera->CreateProjectionMatrix(fAspectRatio);
	}
}