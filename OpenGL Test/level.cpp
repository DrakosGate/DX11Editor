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
#include <thread>
#include <rapidxml_utils.hpp>

// Local Includes
#include "defines.h"
#include "clock.h"
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
#include "threadpool.h"
#include "openclkernel.h"
#include "fontrenderer.h"

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
, m_pDepthMRT(0)
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
, m_pResourceManager(0)
, m_pThreadPool(0)
, m_pOpenCLKernel(0)
, m_pSelectedObject(0)
, m_pFont(0)
, m_bCreateObject(false)
, m_bHasSelectedObject(false)
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
	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = 0;
	}
	if (m_pThreadPool)
	{
		delete m_pThreadPool;
		m_pThreadPool = 0;
	}
	if (m_pOpenCLKernel)
	{
		delete m_pOpenCLKernel;
		m_pOpenCLKernel = 0;
	}
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
	for (unsigned int iEntity = 0; iEntity < m_pLevelEntities.size(); ++iEntity)
	{
		delete m_pLevelEntities[iEntity];
		m_pLevelEntities[iEntity] = 0;
	}
	m_pLevelEntities.clear();
	if (m_pTerrain)
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
	if (m_pDepthMRT)
	{
		delete m_pDepthMRT;
		m_pDepthMRT = 0;
	}
	if (m_pMRT)
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
CLevel::Initialise(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext, CDirectXRenderer* _pRenderer, HWND _hWindow, TInputStruct* _pInput, int _iScreenWidth, int _iScreenHeight)
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
	CreateEntities(_pDevice, _pDevContext, _hWindow);

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
	for(unsigned int i = 0; i < 3; ++i)
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

	m_eRenderState = RENDERSTATE_DEBUG;

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
CLevel::CreateEntities(ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext, HWND _hWindow)
{	
	printf("================= Creating Entities =================\n");
	//Create entity manager to contain all objects
	m_pEntityManager = new CEntityManager();
	m_pEntityManager->Initialise(_pDevice);

	//Create thread pool for parallel task management
	m_pThreadPool = new CThreadPool();
	//std::thread::hardware_concurrency() is the recommended thread usage for this system
	m_pThreadPool->Initialise(2, 20);// std::thread::hardware_concurrency(), 20);

	m_pOpenCLKernel = new COpenCLKernel();
	m_pOpenCLKernel->InitialiseOpenCL();
	m_pOpenCLKernel->LoadProgram("OpenCLKernels/test.cl");
	m_pOpenCLKernel->SendDataToGPU();
	m_pOpenCLKernel->Run();

	//Read level resources
	m_pResourceManager = new CResourceManager();
	m_pResourceManager->Initialise(_pDevice, "Data/Resources.xml");
	m_pResourceManager->LoadPrefabTypes(_pDevice, m_pEntityManager, "Data/Prefabs.xml");
	
	//Load font
	m_pFont = new CFontRenderer();
	m_pFont->Initialise("Something", 16, 6);
	m_pFont->Write("This is a message345", D3DXVECTOR3(10.0f, WINDOW_HEIGHT * 0.1f, 0.0f), D3DXVECTOR2(15.0f, 20.0f));
	m_pFont->SetObjectShader(&m_pShaderCollection[SHADER_FONT]);
	m_pFont->SetDiffuseMap(m_pResourceManager->GetTexture(std::string("font_arial")));
	m_pEntityManager->AddEntity(m_pFont, SCENE_FONT);

	int iCurrentPixel = 0;
	int iTextureWidth = 256;
	int iTextureHeight = 256;
	std::string sTextureName = "terraingradient";
	TUCHARColour* pTerrainTexture = new TUCHARColour[iTextureWidth * iTextureHeight];
	for (int iHeight = 0; iHeight < iTextureHeight; ++iHeight)
	{
		for (int iWidth = 0; iWidth < iTextureWidth; ++iWidth)
		{
			float fRandom = (rand() % 25500) * 0.01f;
			pTerrainTexture[iCurrentPixel].r = static_cast<unsigned char>(fRandom);
			pTerrainTexture[iCurrentPixel].g = static_cast<unsigned char>(fRandom);
			pTerrainTexture[iCurrentPixel].b = static_cast<unsigned char>(fRandom);
			pTerrainTexture[iCurrentPixel].a = 255;
			++iCurrentPixel;
		}
	}
	m_pResourceManager->CreateTextureFromData(_pDevice, reinterpret_cast<unsigned char*>(pTerrainTexture), sTextureName, iTextureWidth, iTextureHeight);
	delete[] pTerrainTexture;

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
	m_pTerrain->SetDiffuseMap(m_pResourceManager->GetTexture(std::string("grass")));
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
	m_pCursor->SetModel(m_pResourceManager->GetModel(std::string("cursor")));
	m_pCursor->SetDiffuseMap(m_pResourceManager->GetTexture(std::string("cursor")));
	m_pCursor->SetObjectShader(&m_pShaderCollection[SHADER_MRT]);
	m_pCursor->SetPosition(D3DXVECTOR3(0.0f, 0.4f, 0.0f));
	m_pCursor->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	m_pCursor->SetRotation(D3DXVECTOR3(0.0f, -0.5f, 0.0f));
	m_pEntityManager->AddEntity(m_pCursor, SCENE_3DSCENE); 

	//Setup AI
	m_pHivemind = new CAIHiveMind();
	m_pHivemind->Initialise();
	m_pHivemind->CreateNavigationGrid(_pDevice, m_pEntityManager, &m_pShaderCollection[SHADER_POINTSPRITE], 20.0f, 60, 60);

	//Load Default level data
	LoadLevel(_pDevice, "Data/Levels/level1.xml");

	m_pEditor = new CEditorInterface();
	m_pEditor->Initialise(_hWindow, this);
	m_pEditor->LoadFromXML(_pDevice, m_pResourceManager, "Data/EditorLayout.xml");
	m_pEditor->SetObjectShader(&m_pShaderCollection[SHADER_POINTSPRITE]);
	m_pEditor->SetDiffuseMap(m_pResourceManager->GetTexture(std::string("menu_button")));
	m_pEntityManager->AddEntity(m_pEditor, SCENE_UI);

	m_pSelectionCursor = new CModel();
	m_pSelectionCursor->Initialise();
	m_pSelectionCursor->LoadSquare(_pDevice, 1.0f, D3DXVECTOR2(1.0f, 1.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pSelectionCursor->SetPosition(D3DXVECTOR3(0.0f, 0.4f, 0.0f));
	m_pSelectionCursor->SetObjectShader(&m_pShaderCollection[SHADER_MRT]);
	m_pSelectionCursor->SetDiffuseMap(m_pResourceManager->GetTexture(std::string("selectionbox")));
	m_pSelectionCursor->ToggleBillboarded(true);
	m_pEntityManager->AddEntity(m_pSelectionCursor, SCENE_3DSCENE);

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
CLevel::Process(ID3D11Device* _pDevice, CClock* _pClock, float _fDeltaTime)
{
	ProcessInput(_pDevice, _fDeltaTime);

	m_pCamera->Process(_fDeltaTime);
	m_pOrthoCamera->Process(_fDeltaTime);

	//for (unsigned int i = 0; i < m_pHumans.size(); ++i)
	//{
	//	m_pLightManager->GetPoint(i)->SetPosition(m_pHumans[i]->GetPosition() + m_pHumans[i]->GetForward() * 0.5f);
	//}
	//m_pLightManager->GetPoint(m_pLightManager->GetLightCount(LIGHT_POINT) - 1)->SetPosition(m_pCursor->GetPosition() + D3DXVECTOR3(0.0f, 0.1f, 0.0f));
	
	//Process camera and movement
	//m_pPlayer->ProcessInput(_fDeltaTime);
	m_pEditor->RefreshBuffers(_pDevice);
	m_pEditor->Process(_fDeltaTime);
	
	//Process entities
	if (m_pEditor->IsActive() == false)
	{
		m_pHivemind->Process(m_pThreadPool, _fDeltaTime);
	}
	//Process entity selection
	if (m_bHasSelectedObject)
	{
		if (m_pInput->bLeftMouseClick.bPressed)
		{
			D3DXVECTOR3 vecCursor = m_pCursor->GetPosition();
			vecCursor.y = m_pSelectedObject->GetScale().y * 0.5f;
			m_pSelectedObject->SetPosition(vecCursor);
		}
		m_pSelectionCursor->SetPosition(m_pSelectedObject->GetPosition());
		m_pSelectionCursor->SetScale(m_pSelectedObject->GetScale());
	}
	
	//m_pResourceManager->GetAnimatedModel("chicken")->Process(_fDeltaTime);
	m_pFont->ProcessFont(_pDevice);

	m_pEntityManager->Process(_fDeltaTime, SCENE_3DSCENE, m_pCamera);
	m_pEntityManager->Process(_fDeltaTime, SCENE_3DANIM, m_pCamera);
	m_pEntityManager->Process(_fDeltaTime, SCENE_UI, m_pCamera);
	m_pEntityManager->Process(_fDeltaTime, SCENE_DEBUG, m_pCamera);
	m_pEntityManager->Process(_fDeltaTime, SCENE_FONT, m_pCamera);
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

	//Print FPS
	char cBuffer[32];
	sprintf_s(cBuffer, 32, "Frame Time Elapsed: %f", _fDeltaTime);
	m_pFont->Write(cBuffer, D3DXVECTOR3(10.0f, WINDOW_HEIGHT * 0.12f, 0.0f), D3DXVECTOR2(15.0f, 20.0f));
}
/**
*
* CLevel class Processes input for all entities in this level
* (Task ID: n/a)
*
* @author Christopher Howlett
* @param _fDeltaTime Game time elapsed
*
*/
bool
CLevel::ProcessInput(ID3D11Device* _pDevice, float _fDeltaTime)
{
	//Toggle level editor
	if (m_pInput->bTilde.bPressed && m_pInput->bTilde.bPreviousState == false)
	{
		m_pEditor->ToggleEditor(!m_pEditor->IsActive());
	}
	bool bMouseOverEditor = m_pEditor->ProcessInput(_pDevice, m_pInput, _fDeltaTime);
	//Check if new objects are being created
	if (m_pEditor->GetEditorState() == EDITOR_SELECTED)
	{
		//Get name of new prefab
		m_sSelectedPrefab = m_pEditor->GetSelectedPrefab();
		m_bCreateObject = true;

		//Change the cursor model
		m_pCursor->SetModel(m_pResourceManager->GetModel(m_sSelectedPrefab));
		m_pCursor->SetDiffuseMap(m_pResourceManager->GetTexture(m_sSelectedPrefab));
	}
	//Create new objects
	if (m_bCreateObject && bMouseOverEditor == false)
	{
		//Instantiate a new entity
		if (m_pInput->bLeftMouseClick.bPressed && m_pInput->bLeftMouseClick.bPreviousState == false)
		{
			m_pLevelEntities.push_back(m_pEntityManager->InstantiatePrefab(	_pDevice,
																			m_pHivemind,
																			m_sSelectedPrefab,
																			&m_pShaderCollection[SHADER_MRT],
																			SCENE_3DSCENE,
																			m_pCursor->GetPosition(),
																			D3DXVECTOR3(1.0f, 1.0f, 1.0f),
																			D3DXVECTOR3(0.0f, static_cast<float>(rand() % 360), 0.0f),
																			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)));
		}
		//Cancel entity creation
		if (m_pInput->bRightMouseClick.bPressed && m_pInput->bRightMouseClick.bPreviousState == false)
		{
			m_bCreateObject = false;
			//Set model back to cursor model
			m_pCursor->SetModel(m_pResourceManager->GetModel(std::string("cursor")));
			m_pCursor->SetDiffuseMap(m_pResourceManager->GetTexture(std::string("cursor")));
		}
	}
	else
	{
		//Check if the cursor has collided with any objects
		if (m_pInput->bLeftMouseClick.bPressed && m_pInput->bLeftMouseClick.bPreviousState == false)
		{
			for (unsigned int iObject = 0; iObject < m_pLevelEntities.size(); ++iObject)
			{
				if (m_pCursor->HasCollided(m_pLevelEntities[iObject]))
				{
					m_pSelectionCursor->SetDoDraw(true);
					m_bHasSelectedObject = true;
					m_pSelectedObject = m_pLevelEntities[iObject];
					break;
				}
			}
		}
		//Cancel entity selection
		if (m_pInput->bRightMouseClick.bPressed && m_pInput->bRightMouseClick.bPreviousState == false)
		{
			m_pSelectionCursor->SetDoDraw(false);
			m_bHasSelectedObject = false;
			m_pSelectedObject = 0;
		}
	}

	//Process Camera Input
	m_pCamera->ProcessInput(m_pInput, D3DXVECTOR2(1.0f, 0.0f), !bMouseOverEditor, _fDeltaTime);
	//Cast ray from camera to ground plane
	m_pInput->m_tMouseRay = m_pCamera->GetMouseRay(m_pInput->vecMouse);
	float fIntersectionPoint = PlaneToLine(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), m_pInput->m_tMouseRay);
	if (fIntersectionPoint > 0.0f)
	{
		m_pCursor->SetPosition(m_pInput->m_tMouseRay.vecPosition + (m_pInput->m_tMouseRay.vecDirection * fIntersectionPoint));
		float fCameraRotation = (static_cast<float>(D3DX_PI)* 0.9f) + atan2f(m_pCamera->GetLook().x, m_pCamera->GetLook().z);
		m_pCursor->SetRotation(D3DXVECTOR3(0.0f, fCameraRotation, 0.0f));
	}
	
	//Toggle rendering mode
	if (m_pInput->bToggleRender.bPressed && m_pInput->bToggleRender.bPreviousState == false)
	{
		if (m_eRenderState == RENDERSTATE_EDITOR)
		{
			m_eRenderState = RENDERSTATE_DEBUG;
		}
		else
		{
			m_eRenderState = RENDERSTATE_EDITOR;
		}
	}

	return true;
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

	//RENDER GAME ENTITIES
	if (m_eRenderState >= RENDERSTATE_GAME)
	{
		//============================ Multiple Render Texture ================================
		//Set shader for rendering
		_pDevice->OMSetDepthStencilState(m_pRenderTargets[RENDER_MRT].GetAddDepthState(), 0);
		_pDevice->OMSetRenderTargets(4, m_pMRT, m_pRenderer->GetDepthStencilView());

		m_pDiffuseMRT->ClearRenderTarget(_pDevice, m_pRenderer->GetDepthStencilView(), m_pClearColour);
		m_pNormalsMRT->ClearRenderTarget(_pDevice, m_pRenderer->GetDepthStencilView(), m_pClearColour);
		m_pPositionMRT->ClearRenderTarget(_pDevice, m_pRenderer->GetDepthStencilView(), m_pClearColour);
		m_pDepthMRT->ClearRenderTarget(_pDevice, m_pRenderer->GetDepthStencilView(), m_pClearColour);

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
		m_pRenderTargets[RENDER_DEFERRED].SetRenderTarget(_pDevice, 1, m_pDiffuseMRT->GetDepthStencilView());
		m_pRenderTargets[RENDER_DEFERRED].ClearRenderTarget(_pDevice, m_pDiffuseMRT->GetDepthStencilView(), m_pClearColour);

		//Send MRT texture data to shader
		ID3D11ShaderResourceView* const texture[3] = {	m_pDiffuseMRT->GetRenderShaderResource(),
														m_pNormalsMRT->GetRenderShaderResource(),
														m_pPositionMRT->GetRenderShaderResource() };
		_pDevice->PSSetShaderResources(0, 3, texture);


		m_pRenderTarget->SetDiffuseMap(m_pDiffuseMRT->GetRenderShaderResource());
		DrawScene(_pDevice, m_pOrthoCamera, SCENE_FINAL);
		m_pRenderTarget->SetDiffuseMap(m_pRenderTargets[RENDER_DEFERRED].GetRenderShaderResource());

		//============================ FINAL PASS ================================
		m_pRenderTargets[RENDER_POST].SetRenderTarget(_pDevice, 1, m_pDiffuseMRT->GetDepthStencilView());
		m_pRenderTargets[RENDER_POST].ClearRenderTarget(_pDevice, m_pDiffuseMRT->GetDepthStencilView(), m_pClearColour);
		_pDevice->VSSetShader(m_pShaderCollection[SHADERPOST_RADIALBLUR].GetVertexShader(), NULL, 0);
		_pDevice->GSSetShader(NULL, NULL, 0);
		_pDevice->PSSetShader(m_pShaderCollection[SHADERPOST_RADIALBLUR].GetPixelShader(), NULL, 0);
		DrawScene(_pDevice, m_pOrthoCamera, SCENE_FINAL);
		m_pRenderTarget->SetDiffuseMap(m_pRenderTargets[RENDER_POST].GetRenderShaderResource());

		m_pRenderer->PrepareLastScene();
		//Prepare swapchain buffers
		_pDevice->VSSetShader(m_pShaderCollection[SHADER_FINALOUTPUT].GetVertexShader(), NULL, 0);
		_pDevice->GSSetShader(NULL, NULL, 0);
		_pDevice->PSSetShader(m_pShaderCollection[SHADER_FINALOUTPUT].GetPixelShader(), NULL, 0);
		DrawScene(_pDevice, m_pOrthoCamera, SCENE_FINAL);
	}
	_pDevice->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	_pDevice->IASetInputLayout(m_pVertexLayout[VERTEX_FONT]);
	//Render debug lines to screen
	_pDevice->VSSetShader(m_pShaderCollection[SHADER_FONT].GetVertexShader(), NULL, 0);
	_pDevice->GSSetShader(m_pShaderCollection[SHADER_FONT].GetGeometryShader(), NULL, 0);
	_pDevice->PSSetShader(m_pShaderCollection[SHADER_FONT].GetPixelShader(), NULL, 0);
	DrawScene(_pDevice, m_pOrthoCamera, SCENE_FONT);

	_pDevice->IASetInputLayout(m_pVertexLayout[VERTEX_POINT]);
	if (m_eRenderState >= RENDERSTATE_DEBUG)
	{
		//Render debug lines to screen
		_pDevice->VSSetShader(m_pShaderCollection[SHADER_LINERENDERER].GetVertexShader(), NULL, 0);
		_pDevice->GSSetShader(m_pShaderCollection[SHADER_LINERENDERER].GetGeometryShader(), NULL, 0);
		_pDevice->PSSetShader(m_pShaderCollection[SHADER_LINERENDERER].GetPixelShader(), NULL, 0);
		DrawScene(_pDevice, m_pCamera, SCENE_DEBUG);
	}
	if (m_eRenderState >= RENDERSTATE_EDITOR)
	{
		//Render UI objects to screen
		_pDevice->VSSetShader(m_pShaderCollection[SHADER_POINTSPRITE].GetVertexShader(), NULL, 0);
		_pDevice->GSSetShader(m_pShaderCollection[SHADER_POINTSPRITE].GetGeometryShader(), NULL, 0);
		_pDevice->PSSetShader(m_pShaderCollection[SHADER_POINTSPRITE].GetPixelShader(), NULL, 0);
		m_pResourceManager->SendTextureDataToShader(_pDevice);
		DrawScene(_pDevice, m_pOrthoCamera, SCENE_UI);
	}

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
	m_pRenderTargets[RENDER_MRT].Initialise(_pDevice, true, false, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pRenderTargets[RENDER_DEFERRED].Initialise(_pDevice, true, false, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pRenderTargets[RENDER_POST].Initialise(_pDevice, true, false, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pRenderTargets[RENDER_FINAL].Initialise(_pDevice, true, false, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	
	m_pDiffuseMRT = new CRenderToTexture();
	m_pDiffuseMRT->Initialise(_pDevice, true, false, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pNormalsMRT = new CRenderToTexture();
	m_pNormalsMRT->Initialise(_pDevice, true, false, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pPositionMRT = new CRenderToTexture();
	m_pPositionMRT->Initialise(_pDevice, true, false, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pDepthMRT = new CRenderToTexture();
	m_pDepthMRT->Initialise(_pDevice, true, false, static_cast<int>(WINDOW_WIDTH), static_cast<int>(WINDOW_HEIGHT));
	m_pMRT = new ID3D11RenderTargetView*[4];
	m_pMRT[0] = m_pDiffuseMRT->GetRenderTarget();
	m_pMRT[1] = m_pNormalsMRT->GetRenderTarget();
	m_pMRT[2] = m_pPositionMRT->GetRenderTarget();
	m_pMRT[3] = m_pDepthMRT->GetRenderTarget();
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

	m_pShaderCollection[SHADER_FONT].Initialise(_pDevice);
	m_pShaderCollection[SHADER_FONT].CompileVertexShader(_pDevice, L"Shaders/fontshader_vs.hlsl", "FontVS");
	m_pShaderCollection[SHADER_FONT].CompileGeometryShader(_pDevice, L"Shaders/fontshader_gs.hlsl", "FontGS");
	m_pShaderCollection[SHADER_FONT].CompilePixelShader(_pDevice, L"Shaders/fontshader_ps.hlsl", "FontPS");


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

	D3D11_INPUT_ELEMENT_DESC fontDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SCALE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UVTOPLEFT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UVBOTTOMRIGHT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	iNumElements = 5;

	// Create the input layout
	HRCheck(_pDevice->CreateInputLayout(fontDesc,
										iNumElements,
										m_pShaderCollection[SHADER_FONT].GetShaderBlob()->GetBufferPointer(),
										m_pShaderCollection[SHADER_FONT].GetShaderBlob()->GetBufferSize(), //CHANGE THIS!!!
										&m_pVertexLayout[VERTEX_FONT]),
										L"Could not create font input layout");

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
void
CLevel::LoadLevel(ID3D11Device* _pDevice, char* _pcLevelFilename)
{
	//Delete all current entities in the scene
	for (unsigned int iEntity = 0; iEntity < m_pLevelEntities.size(); ++iEntity)
	{
		delete m_pLevelEntities[iEntity];
		m_pLevelEntities[iEntity] = 0;
	}
	m_pLevelEntities.clear();

	//Open file containing level information
	rapidxml::file<> xmlFile(_pcLevelFilename);
	rapidxml::xml_document<> xmlDoc;

	//Parse file string
	xmlDoc.parse<0>(xmlFile.data());
	rapidxml::xml_node<>* pRoot = xmlDoc.first_node();

	//Loop through models
	printf("\n  == LOADING LEVEL FROM FILE: %s ==\n", _pcLevelFilename);
	for (rapidxml::xml_node<>* pCurrentPrefab = pRoot->first_node("prefab"); pCurrentPrefab; pCurrentPrefab = pCurrentPrefab->next_sibling())
	{
		//Get prefab type
		std::string sType = pCurrentPrefab->first_node("type")->value();

		//Get Position Scale and Rotation data
		D3DXVECTOR3 vecPosition(ReadFromString<float>(pCurrentPrefab->first_node("position")->first_attribute("x")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("position")->first_attribute("y")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("position")->first_attribute("z")->value()));
		D3DXVECTOR3 vecScale(	ReadFromString<float>(pCurrentPrefab->first_node("scale")->first_attribute("x")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("scale")->first_attribute("y")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("scale")->first_attribute("z")->value()));
		D3DXVECTOR3 vecRotation(ReadFromString<float>(pCurrentPrefab->first_node("rotation")->first_attribute("x")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("rotation")->first_attribute("y")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("rotation")->first_attribute("z")->value()));
		//Get Prefab colour
		D3DXCOLOR prefabColour(	ReadFromString<float>(pCurrentPrefab->first_node("colour")->first_attribute("r")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("colour")->first_attribute("g")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("colour")->first_attribute("b")->value()),
								ReadFromString<float>(pCurrentPrefab->first_node("colour")->first_attribute("a")->value()));
		
		CPrefab* pNewPrefab = m_pEntityManager->InstantiatePrefab(	_pDevice,
																	m_pHivemind,
																	sType,
																	&m_pShaderCollection[SHADER_MRT],
																	SCENE_3DSCENE,
																	vecPosition,
																	vecScale,
																	vecRotation,
																	prefabColour);
		m_pLevelEntities.push_back(pNewPrefab);
	}
}
void
CLevel::SaveLevel(ID3D11Device* _pDevice, char* _pcLevelFilename)
{
	
}