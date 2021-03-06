//
//  File Name   :   level.cpp
//  Description :   Code for Level
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <D3D11.h>
#include <thread>
#include <rapidxml_print.hpp>

// Local Includes
#include "defines.h"
#include "clock.h"
#include "entitymanager.h"
#include "model.h"
#include "animatedmodel.h"
#include "grass.h"
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
#include "dx11renderer.h"
#include "audioplayer.h"
#include "resourcemanager.h"
#include "editorinterface.h"
#include "threadpool.h"
//#include "openclcontext.h"
#include "fontrenderer.h"
#include "network.h"
#include "performancegraph.h"
#include "aicontroller.h"
#include "scenehierarchy.h"

// This Include
#include "level.h"

// Static Variables

// Static Function Prototypes

// Implementation

Level::Level()
	: m_pRenderer( 0 )
	, m_eGameScene( SCENE_3DSCENE )
	, m_eGrassProcessingMethod( PROCESSING_SEQUENTIAL )
	, m_eAIProcessingMethod( PROCESSING_SEQUENTIAL )
	, m_pEntityManager( 0 )
	//, m_pPlayer( nullptr )
	, m_pCursor( 0 )
	, m_pTerrain( 0 )
	, m_pGrass( 0 )
	, m_pGrassJobs( 0 )
	, m_pHivemind( 0 )
	, m_pCamera( 0 )
	, m_pOrthoCamera( 0 )
	, m_pLightManager( 0 )
	, m_pRenderTargets( 0 )
	, m_pDiffuseMRT( 0 )
	, m_pNormalsMRT( 0 )
	, m_pPositionMRT( 0 )
	, m_pDepthMRT( 0 )
	, m_pMRT( 0 )
	, m_pRenderTarget( 0 )
	, m_pRenderMonitor( 0 )
	, m_pShaderCollection( 0 )
	, m_pVertexLayout( 0 )
	, m_pSelectionCursor( 0 )
	, m_fGrassScale( 0.0f )
	, m_fGameTimeElapsed( 0.0f )
	, m_iScreenWidth( 0 )
	, m_iScreenHeight( 0 )
	, m_pResourceManager( 0 )
	, m_pThreadPool( 0 )
	//, m_pCLKernel( 0 )
	, m_pSelectedObject( 0 )
	, m_pFont( 0 )
	, m_pGraph( 0 )
	, m_iGraphDelay( 0 )
	, m_pSceneHierarchy( 0 )
	, m_pcProcessingMethodName( 0 )
	, m_iThreadCount( 0 )
	, m_bCreateObject( false )
	, m_bHasSelectedObject( false )
	, m_eGrassState( GRASS_OFF )
	, m_pSetupData( 0 )
	, m_bInputIsEnabled( true )
{
	m_matWorldViewProjection = Math::MatrixIdentity();
}

Level::~Level()
{
	Network::DestroyInstance();

	//Wait for parallel processes to finish
	SAFEDELETE( m_pThreadPool );
	//SAFEDELETE( m_pCLKernel );

	//Cleanup singletons
	AudioPlayer::GetInstance().DestroyInstance();

	for( unsigned int iEntity = 0; iEntity < m_pLevelEntities.size(); ++iEntity )
	{
		SAFEDELETE( m_pLevelEntities[ iEntity ] );
	}
	m_pLevelEntities.clear();
	m_vecGrassEntities.clear();
	SAFEDELETE( m_pSceneHierarchy );
	SAFEDELETE( m_pGraph );
	SAFEDELETE( m_pGrass );
	SAFEDELETE( m_pEditor );
	SAFEDELETE( m_pCursor );
	SAFEDELETE( m_pSelectionCursor );
	SAFEDELETE( m_pLightManager );
	SAFEDELETE( m_pHivemind );
	SAFEDELETE( m_pTerrain );
	SAFEDELETE( m_pCamera );
	SAFEDELETE( m_pOrthoCamera );
	SAFEDELETEARRAY( m_pFont );
	SAFEDELETEARRAY( m_pGrassJobs );

	//Clear render targets
	SAFEDELETE( m_pDiffuseMRT );
	SAFEDELETE( m_pNormalsMRT );
	SAFEDELETE( m_pPositionMRT );
	SAFEDELETE( m_pDepthMRT );
	SAFEDELETE( m_pRenderTarget );
	SAFEDELETEARRAY( m_pMRT );
	SAFEDELETEARRAY( m_pRenderTargets );

	SAFEDELETE( m_pEntityManager );
	SAFEDELETE( m_pResourceManager );
	SAFEDELETEARRAY( m_pRenderMonitor );
	SAFEDELETEARRAY( m_pcProcessingMethodName );

	//Clear shaders
	SAFEDELETEARRAY( m_pShaderCollection );

	//Clear COM Objects
	ReleaseCOM( m_pRasteriserState );
	ReleaseCOM( m_pGrassRasteriser );
	ReleaseCOM( m_pSamplerState );
	for( int iLayout = 0; iLayout < VERTEX_MAX; ++iLayout )
	{
		ReleaseCOM( m_pVertexLayout[ iLayout ] );
	}
	SAFEDELETEARRAY( m_pVertexLayout );
}

bool
Level::Initialise( DX11Renderer* _pRenderer, TSetupStruct* _pSetupData, HWND _hWindow, int _iScreenWidth, int _iScreenHeight )
{
	SetFocus( GetConsoleWindow() );
	m_pSetupData = _pSetupData;

	//Set initial processing values
	m_eGrassProcessingMethod = _pSetupData->eGrassProcessing;
	m_eAIProcessingMethod = _pSetupData->eAIProcessing;
	m_eGrassState = _pSetupData->eGrassState;
	m_eRenderState = _pSetupData->eRenderState;

	//Disable input while logging
	if( m_pSetupData->bDoLog )
	{
		m_bInputIsEnabled = false;
	}

	m_pRenderer = _pRenderer;
	m_iScreenWidth = _iScreenWidth;
	m_iScreenHeight = _iScreenHeight;

	m_pClearColour[ 0 ] = 0.01f;
	m_pClearColour[ 1 ] = 0.01f;
	m_pClearColour[ 2 ] = 0.1f;
	m_pClearColour[ 3 ] = 1.0f;

	LoadShaderData( m_pRenderer->GetDevice(), m_pRenderer->GetDeviceContext() );
	BuildLevelVertexLayouts( m_pRenderer->GetDevice(), m_pRenderer->GetDeviceContext() );

	//Create Perspective camera
	float fAspectRatio = static_cast<float>( m_iScreenWidth ) / static_cast<float>( m_iScreenHeight );
	m_pCamera = new Camera();
	m_pCamera->Initialise( 20.0f, 0.2f, 10.0f, m_iScreenWidth, m_iScreenHeight, true );
	m_pCamera->SetPosition( Math::Vector3( 0.0f, 2.0f, -15.0f ) );
	if( m_pSetupData->bDoLog )
	{
		m_pCamera->SetPosition( Math::Vector3( 0.0f, 10.0f, -15.0f ) );
	}
	Math::Vector3 vecCameraLook;
	vecCameraLook = Math::Normalise( ( Math::Vector3( 0.0f, 0.0f, 0.0f ) - m_pCamera->GetPosition() ) );
	m_pCamera->SetLook( vecCameraLook );
	m_pCamera->CreateProjectionMatrix( fAspectRatio );

	//Create Othographic camera
	m_pOrthoCamera = new Camera();
	m_pOrthoCamera->Initialise( 0.0f, 0.0f, 0.0f, static_cast<int>( m_iScreenWidth ), static_cast<int>( m_iScreenHeight ), false );
	m_pOrthoCamera->SetPosition( Math::Vector3( 0.0f, 0.0f, -1000.0f ) );
	vecCameraLook = Math::Normalise( ( Math::Vector3( 0.0f, 0.0f, 0.0f ) - m_pOrthoCamera->GetPosition() ) );
	m_pOrthoCamera->SetLook( vecCameraLook );
	m_pOrthoCamera->CreateProjectionMatrix( fAspectRatio );

	//Create render targets and screen monitors
	CreateRenderTargets( m_pRenderer->GetDevice() );

	//Create renderable entities and add to entitymanager
	CreateEntities( m_pRenderer->GetDevice(), m_pRenderer->GetDeviceContext(), _hWindow );

	AudioPlayer::GetInstance().Initialise( m_pSetupData->bPlaySound );
	AudioPlayer::GetInstance().Play3DSound( SOUND_BIRDCHIRP, Math::Vector3( -1.0f, 0.0f, 0.0f ) );

	float fFieldOfView = static_cast<float>( PI )* 0.25f;
	float fScreenAspect = static_cast<float>( _iScreenWidth ) / static_cast<float>( _iScreenHeight );
	m_matProjection = Math::MatrixPerspectiveFovLH( fFieldOfView, fScreenAspect, 0.1f, 1000.0f );
	m_matWorld = Math::MatrixIdentity();

	char cBuffer[ 64 ];
	m_pcProcessingMethodName = new std::string[ PROCESSING_MAX ];
	m_pcProcessingMethodName[ PROCESSING_SEQUENTIAL ] = "Sequential";
	sprintf_s( cBuffer, 64, "Thread Pool (%i Threads)", m_iThreadCount );
	m_pcProcessingMethodName[ PROCESSING_THREADPOOL ] = cBuffer;
	//	m_pcProcessingMethodName[ PROCESSING_OPENCL ] = "GPU [OpenCL]";
	m_pcProcessingMethodName[ PROCESSING_DISTRIBUTED ] = "Distributed";

	SetFocus( _hWindow );

	return true;
}

void
Level::CreateEntities( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext, HWND _hWindow )
{
	printf( "================= Creating Entities =================\n" );
	//Create entity manager to contain all objects
	m_pEntityManager = new EntityManager();
	m_pEntityManager->Initialise( _pDevice );

	//Read level resources
	m_pSceneHierarchy = new SceneHierarchy();
	m_pSceneHierarchy->Initialise( "Data/Resources.xml" );

	//Load scene resources
	m_pResourceManager = new ResourceManager();
	m_pResourceManager->Initialise( _pDevice, _pDevContext, m_pEntityManager, m_pSceneHierarchy );

	//Initialise lighting
	m_pLightManager = new LightManager();
	m_pLightManager->Initialise();

	//KEY AREA: Setup OpenCL
	//m_pCLKernel = new OpenCLContext();
	//m_pCLKernel->InitialiseOpenCL();

	//KEY AREA: Setup Thread Pool
	m_pThreadPool = new ThreadPool();
	m_iThreadCount = std::thread::hardware_concurrency();
	//std::thread::hardware_concurrency() is the recommended thread usage for this system
	m_pThreadPool->Initialise( m_iThreadCount );

	//Setup AI Hivemind
	m_pHivemind = new AIHiveMind();
	m_pHivemind->Initialise( m_pSetupData->iAStarSearchDepth );
	m_pHivemind->CreateNavigationGrid( _pDevice, m_pEntityManager, &m_pShaderCollection[ SHADER_POINTSPRITE ], 20.0f, 40, 40 );
	m_pEntityManager->SetLevelInformation( m_pHivemind, m_pLightManager );

	//Load font
	m_pFont = new FontRenderer[ FONT_MAX ];
	m_pFont[ FONT_DEBUG ].Initialise( "Something", 16, 6, Math::Vector3( 10.0f, WINDOW_HEIGHT * 0.1f, 2.0f ), Math::Vector2( 12.0f, 15.0f ), Math::Colour( 0.5f, 0.5f, 1.0f, 1.0f ) );
	m_pFont[ FONT_SCENEGRAPH ].Initialise( "Something", 16, 6, Math::Vector3( WINDOW_WIDTH * 0.81f, WINDOW_HEIGHT * 0.11f, 0.0f ), Math::Vector2( 12.0f, 15.0f ), Math::Colour( 1.0f, 0.0f, 0.0f, 1.0f ) );
	m_pFont[ FONT_PERFORMANCE ].Initialise( "Something", 16, 6, Math::Vector3( WINDOW_WIDTH * 0.01f, WINDOW_HEIGHT * 0.41f, 0.6f ), Math::Vector2( 10.0f, 12.0f ), Math::Colour( 0.7f, 0.9f, 0.7f, 1.0f ) );
	for( int iFont = 0; iFont < FONT_MAX; ++iFont )
	{
		m_pFont[ iFont ].SetObjectShader( &m_pShaderCollection[ SHADER_FONT ] );
		m_pFont[ iFont ].SetDiffuseMap( m_pResourceManager->GetTexture( std::string( "font_arial" ) ) );
		m_pEntityManager->AddEntity( &m_pFont[ iFont ], SCENE_FONT );
	}

	m_pRenderTarget = new Model();
	m_pRenderTarget->Initialise();
	m_pRenderTarget->LoadSquare( _pDevice, 1.0f, Math::Vector2( 1.0f, 1.0f ), Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_pRenderTarget->SetObjectShader( &m_pShaderCollection[ SHADER_FINALOUTPUT ] );
	m_pRenderTarget->SetScale( Math::Vector3( WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f ) );
	m_pEntityManager->AddEntity( m_pRenderTarget, SCENE_FINAL );

	m_pTerrain = new Model();
	m_pTerrain->Initialise();
	m_pTerrain->LoadSquare( _pDevice, 100.0f, Math::Vector2( 100.0f, 100.0f ), Math::Colour( 1.0f, 0.9f, 1.0f, 1.0f ) );
	m_pTerrain->SetRotation( Math::Vector3( ( static_cast<float>( PI ) * 0.5f ), 0.0f, 0.0f ) );
	m_pTerrain->SetObjectShader( &m_pShaderCollection[ SHADER_MRT ] );
	m_pTerrain->SetDiffuseMap( m_pResourceManager->GetTexture( std::string( "grass" ) ) );
	m_pEntityManager->AddEntity( m_pTerrain, SCENE_PERMANENTSCENE );
	m_pTerrain->SetRadius( FLT_MAX );

	//Set the terrain to be the root node
	m_pTerrain->SetEntityType( std::string( "terrain" ) );
	m_pRootNode = m_pTerrain->CreateNode( NULL );

	//Create grass object
	m_fGrassScale = 20.0f;
	m_pGrass = new Grass();
	m_pGrass->Initialise( _pDevice, m_pResourceManager, m_pSetupData->iGrassDimensions, m_fGrassScale, Math::Vector2( 10.0f, 10.0f ), Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f ), m_iThreadCount );
	m_pGrass->SetObjectShader( &m_pShaderCollection[ SHADER_GRASS ] );
	m_pGrass->SetDiffuseMap( m_pResourceManager->GetTexture( std::string( "grassblades" ) ) );
	m_pGrass->SetRadius( FLT_MAX );
	m_pEntityManager->AddEntity( m_pGrass, SCENE_GRASS );
	m_pGrassJobs = new TGrassThread[ m_iThreadCount ];
	for( int iJob = 0; iJob < m_iThreadCount; ++iJob )
	{
		m_pGrassJobs[ iJob ] = TGrassThread( m_pGrass, iJob, 0.0f );
	}

	//Load Default level data
	LoadLevel( _pDevice, m_pSetupData->pcDefaultLevel );

	m_pCursor = new Prefab();
	m_pCursor->Initialise( _pDevice, 1.0f );
	m_pCursor->SetModel( m_pResourceManager->GetModel( std::string( "cursor" ) ) );
	m_pCursor->SetDiffuseMap( m_pResourceManager->GetTexture( std::string( "cursor" ) ) );
	m_pCursor->SetObjectShader( &m_pShaderCollection[ SHADER_MRT ] );
	m_pCursor->SetPosition( Math::Vector3( 0.0f, 0.4f, 0.0f ) );
	m_pCursor->SetScale( Math::Vector3( 0.5f, 0.5f, 0.5f ) );
	m_pCursor->SetRotation( Math::Vector3( 0.0f, -0.5f, 0.0f ) );
	m_pCursor->SetEntityType( std::string( "PlayerCursor" ) );
	m_pCursor->CreateNode( m_pRootNode );
	m_pCursor->GetNode()->vecLights.push_back( m_pLightManager->AddPoint( Math::Vector3( 0.0f, 0.4f, 0.0f ), Math::Colour( 0.3f, 0.3f, 0.8f, 1.0f ), Math::Vector3( 0.05f, 0.2f, 4.0f ), 100.0f ) );
	m_pEntityManager->AddEntity( m_pCursor, SCENE_PERMANENTSCENE );

	//KEY AREA: Create the level editor interface
	m_pEditor = new EditorInterface();
	m_pEditor->Initialise( _hWindow, this );
	m_pEditor->LoadFromXML( _pDevice, m_pResourceManager, m_pEntityManager, "Data/EditorLayout.xml" );
	m_pEditor->SetObjectShader( &m_pShaderCollection[ SHADER_POINTSPRITE ] );
	m_pEntityManager->AddEntity( m_pEditor, SCENE_UI );

	//KEY AREA: Create performance graph
	m_pGraph = new PerformanceGraph();
	m_pGraph->Initialise( _pDevice,
		Math::Vector3( WINDOW_WIDTH * 0.01f, WINDOW_HEIGHT * 0.21f, 0.0f ),
		Math::Vector3( WINDOW_WIDTH * 0.3f, WINDOW_HEIGHT * 0.2f, 1.0f ),
		100 ); //Number of measurements on graph
	m_pGraph->SetGraphRange( 0.0f, 0.0000000001f );
	m_pGraph->SetObjectShader( &m_pShaderCollection[ SHADER_POINTSPRITE ] );
	m_pGraph->SetDiffuseMap( m_pResourceManager->GetTexture( std::string( "menu_button" ) ) );
	if( m_pSetupData->bDoLog )
	{
		m_pGraph->LogPerformance( m_pSetupData->pcLogFilename, m_pSetupData->pcLogDescription, m_pSetupData->iLogFrameDuration );
	}
	m_pEntityManager->AddEntity( m_pGraph, SCENE_UI );

	//Set initial graph delay
	m_iGraphDelay = m_pSetupData->iLogFrameSkip;

	//Create a selection cursor to indicate the selected model
	m_pSelectionCursor = new Model();
	m_pSelectionCursor->Initialise();
	m_pSelectionCursor->LoadSquare( _pDevice, 1.0f, Math::Vector2( 1.0f, 1.0f ), Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f ) );
	m_pSelectionCursor->SetPosition( Math::Vector3( 0.0f, 0.4f, 0.0f ) );
	m_pSelectionCursor->SetRotation( Math::Vector3( static_cast<float>( PI ) * 0.5f, 0.0f, 0.0f ) );
	m_pSelectionCursor->SetObjectShader( &m_pShaderCollection[ SHADER_MRT ] );
	m_pSelectionCursor->SetDiffuseMap( m_pResourceManager->GetTexture( std::string( "selectionbox" ) ) );
	m_pSelectionCursor->ToggleBillboarded( false );
	m_pSelectionCursor->SetDoDraw( false );
	m_pEntityManager->AddEntity( m_pSelectionCursor, SCENE_PERMANENTSCENE );


	//KEY AREA: Setup network for distributed processing
	//m_pNetwork = Network::GetInstance();
	//m_pNetwork->Initialise( m_pGrass, m_pHivemind );

	_pDevContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	_pDevContext->IASetInputLayout( m_pVertexLayout[ VERTEX_STATIC ] );
	_pDevContext->PSSetSamplers( 0, 1, &m_pSamplerState );
}

void
Level::Process( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDeviceContext, Clock* _pClock, float _fDeltaTime )
{
	_pClock->StartTimer();

	m_pCamera->Process( _fDeltaTime );
	m_pOrthoCamera->Process( _fDeltaTime );
	m_pNetwork->Process( _fDeltaTime );

	//Process camera and movement
	//m_pPlayer->ProcessInput(_fDeltaTime);
	m_pEditor->RefreshBuffers( _pDevice );
	m_pEditor->Process( _fDeltaTime );

	//Process entities
	if( m_pEditor->IsActive() == false )
	{
		//	m_pHivemind->GetAI( nullptr )->SetAStarTarget(m_pCursor->GetPosition());
		m_pHivemind->Process( m_pThreadPool, _fDeltaTime );
	}

	//m_pResourceManager->GetAnimatedModel("chicken")->Process(_fDeltaTime);
	for( int iFont = 0; iFont < FONT_MAX; ++iFont )
	{
		m_pFont[ iFont ].ProcessFont( _pDevice );
	}

	m_pRootNode->pEntity->Process( _fDeltaTime, NULL );
	m_pEntityManager->Process( _fDeltaTime, SCENE_3DANIM );
	m_pEntityManager->Process( _fDeltaTime, SCENE_UI );
	m_pEntityManager->Process( _fDeltaTime, SCENE_DEBUG );
	m_pEntityManager->Process( _fDeltaTime, SCENE_FONT );
	m_pEntityManager->Process( _fDeltaTime, SCENE_FINAL );
	m_fGameTimeElapsed += _fDeltaTime;

	if( m_eGrassState == GRASS_DRAWWITHCOLLISIONS )
	{
		m_pEntityManager->Process( _fDeltaTime, SCENE_GRASS );
		float fGrassOffset = m_fGrassScale * 0.5f;
		Math::Vector3 vecGrassPosition = m_pCamera->GetPosition() + ( m_pCamera->GetLook() * fGrassOffset );
		//Math::Vector3 vecGrassPosition = Math::Vector3(0.0f, 0.0f, 0.0f);
		vecGrassPosition.y = 0.0f;
		//Calculate grass offsets
		m_pGrass->SendCollisionData( &m_vecGrassEntities );
		if( m_vecGrassEntities.size() > 0 )
		{
			switch( m_eGrassProcessingMethod )
			{
			case PROCESSING_SEQUENTIAL:
			{
				for( int iSection = 0; iSection < m_iThreadCount; ++iSection )
				{
					m_pGrass->ProcessGrassSection( iSection, _fDeltaTime );
				}
				break;
			}
			case PROCESSING_THREADPOOL:
			{
				for( int iSection = 0; iSection < m_iThreadCount; ++iSection )
				{
					m_pGrassJobs[ iSection ].fDeltaTime = _fDeltaTime;
					m_pThreadPool->AddJobToPool( &GrassProcessingThread, &m_pGrassJobs[ iSection ] );
				}
				break;
			}
			//case PROCESSING_OPENCL:
			//{
			//	m_pGrass->ProcessOpenCL( m_pCLKernel, _fDeltaTime );
			//	break;
			//}
			case PROCESSING_DISTRIBUTED:
			{
				m_pNetwork->SendGrassData( m_pGrass, &m_vecGrassEntities );
				m_pGrass->ProcessDistrubuted( _fDeltaTime );
				break;
			}
			default:
				break;
			};
		}
		//Recreate the grass vertex buffer with new vertex information
		m_pGrass->RecreateGrassMesh( _pDevice,
			_pDeviceContext,
			vecGrassPosition,
			m_vecGrassEntities,
			_fDeltaTime );

		m_pFont[ FONT_DEBUG ].Write( std::string( "Grass State: On With Collisions (Press G to toggle)" ), 3 );
	}
	else
	{
		if( m_eGrassState == GRASS_DRAWONLY )
		{
			m_pFont[ FONT_DEBUG ].Write( std::string( "Grass State: Draw Only (Press G to toggle)" ), 3 );
		}
		else
		{
			m_pFont[ FONT_DEBUG ].Write( std::string( "Grass State: Off (Press G to toggle)" ), 3 );
		}
	}
	//Process audio
	AudioPlayer::GetInstance().SetListenerPosition( Math::Vector3( 0.0f, 0.0f, 0.0f ), m_pCamera->GetLook(), Math::Vector3( 0.0f, 1.0f, 0.0f ) );
	AudioPlayer::GetInstance().Process();

	//Print FPS
	char cBuffer[ 64 ];
	sprintf_s( cBuffer, 64, "FPS: %i", _pClock->GetFPS() );
	m_pFont[ FONT_DEBUG ].Write( std::string( cBuffer ), 0 );
	sprintf_s( cBuffer, 64, "Grass Processing Method: %s", m_pcProcessingMethodName[ m_eGrassProcessingMethod ].c_str() );
	m_pFont[ FONT_DEBUG ].Write( std::string( cBuffer ), 1 );
	sprintf_s( cBuffer, 64, "AI Processing Method: %s", m_pcProcessingMethodName[ m_eAIProcessingMethod ].c_str() );
	m_pFont[ FONT_DEBUG ].Write( std::string( cBuffer ), 2 );

	//KEY AREA: Print Scene hierarchy to screen
	if( m_pEditor->IsActive() )
	{
		int iCurrentTextIndex = 0;
		for( unsigned int iChild = 0; iChild < m_pRootNode->vecChildren.size(); ++iChild )
		{
			if( m_pRootNode->vecChildren[ iChild ]->pEntity->DoDraw() )
			{
				AddTextToSceneGraph( m_pRootNode->vecChildren[ iChild ], iCurrentTextIndex, 0 );
				++iCurrentTextIndex;
			}
		}
	}
	_pClock->EndTimer();
	//KEY AREA: Send data to performance graph
	--m_iGraphDelay;
	if( m_iGraphDelay < 0 )
	{
		m_iGraphDelay = 10;
		float fGraphMeasurement = _pClock->GetTimeElapsed();
		m_pGraph->SetGraphRange( fGraphMeasurement, fGraphMeasurement );
		m_pGraph->AddNode( _pDevice, fGraphMeasurement );
		//Print performance information to screen using fonts
		sprintf_s( cBuffer, 64, "Max:     %f", m_pGraph->GetMax() );
		m_pFont[ FONT_PERFORMANCE ].Write( std::string( cBuffer ), 0 );
		sprintf_s( cBuffer, 64, "Current: %f", fGraphMeasurement );
		m_pFont[ FONT_PERFORMANCE ].Write( std::string( cBuffer ), 1 );
		sprintf_s( cBuffer, 64, "Min:     %f", m_pGraph->GetMin() );
		m_pFont[ FONT_PERFORMANCE ].Write( std::string( cBuffer ), 2 );
	}
}

//bool
//Level::ProcessInput( ID3D11Device* _pDevice, float _fDeltaTime )
//{
//	//Disable input while logging to file
//	if( m_bInputIsEnabled )
//	{
//		//Toggle Grass processing method
//		if( m_pInput->b1.bPressed )
//		{
//			ChangeGrassProcessingMethod( PROCESSING_SEQUENTIAL );
//		}
//		if( m_pInput->b2.bPressed )
//		{
//			ChangeGrassProcessingMethod( PROCESSING_THREADPOOL );
//		}
//		if( m_pInput->b3.bPressed )
//		{
//			ChangeGrassProcessingMethod( PROCESSING_OPENCL );
//		}
//		if( m_pInput->b4.bPressed )
//		{
//			ChangeGrassProcessingMethod( PROCESSING_DISTRIBUTED );
//		}
//		//Toggle AI processing method
//		if( m_pInput->b5.bPressed )
//		{
//			ChangeAIProcessingMethod( PROCESSING_SEQUENTIAL );
//		}
//		if( m_pInput->b6.bPressed )
//		{
//			ChangeAIProcessingMethod( PROCESSING_THREADPOOL );
//		}
//		if( m_pInput->b7.bPressed )
//		{
//			ChangeAIProcessingMethod( PROCESSING_OPENCL );
//		}
//		if( m_pInput->b8.bPressed )
//		{
//			ChangeAIProcessingMethod( PROCESSING_DISTRIBUTED );
//		}
//		if( m_pInput->bG.bPressed && m_pInput->bG.bPreviousState == false )
//		{
//			if( m_eGrassState == GRASS_OFF )
//			{
//				m_eGrassState = GRASS_DRAWONLY;
//			}
//			else if( m_eGrassState == GRASS_DRAWONLY )
//			{
//				m_eGrassState = GRASS_DRAWWITHCOLLISIONS;
//			}
//			else
//			{
//				m_eGrassState = GRASS_OFF;
//			}
//		}
//		//Toggle level editor
//		if( m_pInput->bTilde.bPressed && m_pInput->bTilde.bPreviousState == false )
//		{
//			m_pEditor->ToggleEditor( !m_pEditor->IsActive() );
//		}
//		bool bMouseOverEditor = m_pEditor->ProcessInput( _pDevice, m_pInput, _fDeltaTime );
//		//Check if new objects are being created
//		if( m_pEditor->GetEditorState() == EDITOR_SELECTED )
//		{
//			//Get name of new prefab
//			m_sSelectedPrefab = m_pEditor->GetSelectedPrefab();
//			m_bCreateObject = true;
//
//			//Change the cursor model
//			m_pCursor->SetModel( m_pResourceManager->GetModel( m_sSelectedPrefab ) );
//			m_pCursor->SetDiffuseMap( m_pResourceManager->GetTexture( m_sSelectedPrefab ) );
//		}
//		//KEY AREA: Create new objects from Editor
//		if( m_bCreateObject && bMouseOverEditor == false )
//		{
//			//Instantiate a new entity
//			if( m_pInput->bLeftMouseClick.bPressed && m_pInput->bLeftMouseClick.bPreviousState == false )
//			{
//				//Create scene node information for this object
//				TSceneNode* pNode = new TSceneNode();
//				pNode->tEntity.sPrefabName = m_sSelectedPrefab;
//
//				pNode->tEntity.vecPosition[ 0 ] = m_pCursor->GetPosition().x;
//				pNode->tEntity.vecPosition[ 1 ] = m_pCursor->GetPosition().y;
//				pNode->tEntity.vecPosition[ 2 ] = m_pCursor->GetPosition().z;
//				pNode->tEntity.vecScale[ 0 ] = 1.0f;
//				pNode->tEntity.vecScale[ 1 ] = 1.0f;
//				pNode->tEntity.vecScale[ 2 ] = 1.0f;
//				pNode->tEntity.vecRotation[ 0 ] = 0.0f;
//				pNode->tEntity.vecRotation[ 1 ] = static_cast<float>( rand() % 360 );
//				pNode->tEntity.vecRotation[ 2 ] = 0.0f;
//				pNode->tEntity.colour[ 0 ] = 1.0f;
//				pNode->tEntity.colour[ 1 ] = 1.0f;
//				pNode->tEntity.colour[ 2 ] = 1.0f;
//				pNode->tEntity.colour[ 3 ] = 1.0f;
//
//				CreateObject( _pDevice, pNode, m_pRootNode );
//				SAFEDELETE( pNode );
//			}
//			//Cancel entity creation
//			if( m_pInput->bRightMouseClick.bPressed && m_pInput->bRightMouseClick.bPreviousState == false )
//			{
//				m_bCreateObject = false;
//				//Set model back to cursor model
//				m_pCursor->SetModel( m_pResourceManager->GetModel( std::string( "cursor" ) ) );
//				m_pCursor->SetDiffuseMap( m_pResourceManager->GetTexture( std::string( "cursor" ) ) );
//			}
//		}
//		else
//		{
//			//Check if the cursor has collided with any objects
//			if( m_pInput->bLeftMouseClick.bPressed && m_pInput->bLeftMouseClick.bPreviousState == false )
//			{
//				for( unsigned int iObject = 0; iObject < m_pLevelEntities.size(); ++iObject )
//				{
//					if( m_pCursor->HasCollided( m_pLevelEntities[ iObject ] ) )
//					{
//						m_pSelectionCursor->SetDoDraw( true );
//						m_bHasSelectedObject = true;
//						m_pSelectedObject = m_pLevelEntities[ iObject ];
//						break;
//					}
//				}
//			}
//			//Cancel entity selection
//			if( m_pInput->bRightMouseClick.bPressed && m_pInput->bRightMouseClick.bPreviousState == false )
//			{
//				m_pSelectionCursor->SetDoDraw( false );
//				m_bHasSelectedObject = false;
//				m_pSelectedObject = 0;
//			}
//		}
//
//		//Process Camera Input
//		m_pCamera->ProcessInput( m_pInput, Math::Vector2( 1.0f, 0.0f ), !bMouseOverEditor, _fDeltaTime );
//		//Cast ray from camera to ground plane
//		m_pInput->m_tMouseRay = m_pCamera->GetMouseRay( m_pInput->vecMouse );
//		float fIntersectionPoint = PlaneToLine( Math::Vector3( 0.0f, 0.0f, 0.0f ), Math::Vector3( 0.0f, 1.0f, 0.0f ), m_pInput->m_tMouseRay );
//		if( fIntersectionPoint > 0.0f )
//		{
//			m_pCursor->SetPosition( m_pInput->m_tMouseRay.vecPosition + ( m_pInput->m_tMouseRay.vecDirection * fIntersectionPoint ) );
//			float fCameraRotation = ( static_cast<float>( PI )* 0.9f ) + atan2f( m_pCamera->GetLook().x, m_pCamera->GetLook().z );
//			m_pCursor->SetRotation( Math::Vector3( 0.0f, fCameraRotation, 0.0f ) );
//		}
//
//		//Toggle rendering mode
//		if( m_pInput->bToggleRender.bPressed && m_pInput->bToggleRender.bPreviousState == false )
//		{
//			if( m_eRenderState == RENDERSTATE_EDITOR )
//			{
//				m_eRenderState = RENDERSTATE_DEBUG;
//			}
//			else
//			{
//				m_eRenderState = RENDERSTATE_EDITOR;
//			}
//		}
//
//		//KEY AREA: Process entity selection - Translate, scale, rotate and delete selected objects
//		ProcessEntitySelection( _pDevice, _fDeltaTime );
//	}
//
//	return true;
//}

void
Level::ProcessEntitySelection( ID3D11Device* _pDevice, float _fDeltaTime )
{
	if( m_bHasSelectedObject )
	{
		//if( m_pInput->bLeftMouseClick.bPressed )
		//{
		//	//ROTATE OBJECT
		//	bool bCanMoveObject = true;
		//	if( m_pInput->bShift.bPressed )
		//	{
		//		Math::Vector3 vecToCursor = m_pCursor->GetPosition() - m_pSelectedObject->GetPosition();
		//		vecToCursor.y = 0.0f;
		//		D3DXVec3Normalize( &vecToCursor, &vecToCursor );
		//		m_pSelectedObject->SetForward( vecToCursor );
		//		bCanMoveObject = false;
		//	}
		//	//SCALE OBJECT
		//	if( m_pInput->bCtrl.bPressed )
		//	{
		//		//First time pressing control
		//		float fDistanceFromStart = D3DXVec3LengthSq( &( m_pCursor->GetPosition() - m_pSelectedObject->GetPosition() ) );
		//		if( m_pInput->bCtrl.bPreviousState == false )
		//		{
		//			m_fSelectedObjectScale = m_pSelectedObject->GetScale().x;
		//			m_fSelectedMouseDistance = fDistanceFromStart;
		//		}
		//		float fSelectionScale = m_fSelectedObjectScale * ( fDistanceFromStart - m_fSelectedMouseDistance );
		//		if( fSelectionScale < 0.05f )
		//		{
		//			fSelectionScale = 0.05f;
		//		}
		//		m_pSelectedObject->SetScale( Math::Vector3( fSelectionScale, fSelectionScale, fSelectionScale ) );
		//		bCanMoveObject = false;
		//	}
		//	//TRANSLATE OBJECT
		//	if( bCanMoveObject )
		//	{
		//		//Check if cursor is within drag range
		//		Math::Vector3 vecToObject = m_pSelectedObject->GetPosition() - m_pCursor->GetPosition();
		//		vecToObject.y = 0.0f;
		//		if( D3DXVec3LengthSq( &vecToObject ) < m_pSelectedObject->GetRadius() * m_pSelectedObject->GetRadius() )
		//		{
		//			//Drag object
		//			Math::Vector3 vecCursor = m_pCursor->GetPosition();
		//			vecCursor.y = m_pSelectedObject->GetPosition().y;
		//			m_pSelectedObject->SetPosition( vecCursor );
		//		}
		//	}
		//}
		////Check if user wants to delete this object
		//if( m_pInput->bDelete.bPressed && m_pInput->bDelete.bPreviousState == false )
		//{
		//	m_pSelectedObject->SetDoDraw( false );
		//	m_pSelectedObject = 0;
		//	m_bHasSelectedObject = false;
		//	m_pSelectionCursor->SetDoDraw( false );
		//}
		////Otherwise update cursor position and navigation mesh
		//else
		//{
		//	m_pSelectionCursor->SetPosition( m_pSelectedObject->GetPosition() - Math::Vector3( 0.0f, m_pSelectedObject->GetPosition().y - 0.1f, 0.0f ) );
		//	m_pSelectionCursor->SetScale( Math::Vector3( m_pSelectedObject->GetScale().x, m_pSelectedObject->GetScale().z, 1.0f ) );
		//}
		//m_pHivemind->RecalculateNavGrid( _pDevice );
	}
}

void
Level::Draw( ID3D11DeviceContext* _pDevice )
{
	//Send lighting info to shader
	m_pShaderCollection[ SHADER_OBJECT ].SendLightInformation( _pDevice, m_pLightManager, m_pCamera );

	_pDevice->IASetInputLayout( m_pVertexLayout[ VERTEX_STATIC ] );
	_pDevice->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	//============================ RENDER GAME ENTITIES ================================
	if( m_eRenderState >= RENDERSTATE_GAME )
	{
		//============================ Multiple Render Texture ================================
		//Set shader for rendering
		_pDevice->OMSetDepthStencilState( m_pRenderTargets[ RENDER_MRT ].GetAddDepthState(), 0 );
		_pDevice->OMSetRenderTargets( 4, m_pMRT, m_pRenderer->GetDepthStencilView() );

		m_pDiffuseMRT->ClearRenderTarget( _pDevice, m_pRenderer->GetDepthStencilView(), m_pClearColour );
		m_pNormalsMRT->ClearRenderTarget( _pDevice, m_pRenderer->GetDepthStencilView(), m_pClearColour );
		m_pPositionMRT->ClearRenderTarget( _pDevice, m_pRenderer->GetDepthStencilView(), m_pClearColour );
		m_pDepthMRT->ClearRenderTarget( _pDevice, m_pRenderer->GetDepthStencilView(), m_pClearColour );

		//============================ DRAW ANIMATED CHARACTERS ================================
		_pDevice->IASetInputLayout( m_pVertexLayout[ VERTEX_ANIMATED ] );
		DrawScene( _pDevice, &m_pShaderCollection[ SHADER_ANIMOBJECT ], m_pCamera, SCENE_3DANIM );

		//============================ DRAW STILL CHARACTERS ================================
		_pDevice->IASetInputLayout( m_pVertexLayout[ VERTEX_STATIC ] );
		DrawScene( _pDevice, &m_pShaderCollection[ SHADER_MRT ], m_pCamera, SCENE_3DSCENE );
		DrawScene( _pDevice, &m_pShaderCollection[ SHADER_MRT ], m_pCamera, SCENE_PERMANENTSCENE );

		//============================ DRAW GRASS================================
		if( m_eGrassState != GRASS_OFF )
		{
			m_pRenderer->SetBlendState( BLEND_ALPHATOCOVERAGE );
			_pDevice->RSSetState( m_pGrassRasteriser );
			_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_POINTLIST );
			DrawScene( _pDevice, &m_pShaderCollection[ SHADER_GRASS ], m_pCamera, SCENE_GRASS );
			_pDevice->IASetPrimitiveTopology( D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			_pDevice->RSSetState( m_pRasteriserState );
			m_pRenderer->SetBlendState( BLEND_TRANSPARENT );
		}

		//============================ DEFERRED RENDER ================================
		m_pRenderTargets[ RENDER_DEFERRED ].SetRenderTarget( _pDevice, 1, m_pDiffuseMRT->GetDepthStencilView() );
		m_pRenderTargets[ RENDER_DEFERRED ].ClearRenderTarget( _pDevice, m_pDiffuseMRT->GetDepthStencilView(), m_pClearColour );

		//Send MRT texture data to shader
		ID3D11ShaderResourceView* const texture[ 4 ] = {m_pDiffuseMRT->GetRenderShaderResource(),
														m_pNormalsMRT->GetRenderShaderResource(),
														m_pPositionMRT->GetRenderShaderResource(),
														m_pDepthMRT->GetRenderShaderResource()};
		_pDevice->PSSetShaderResources( 0, 4, texture );

		m_pRenderTarget->SetDiffuseMap( m_pDiffuseMRT->GetRenderShaderResource() );
		DrawScene( _pDevice, &m_pShaderCollection[ SHADER_DEFERRED ], m_pOrthoCamera, SCENE_FINAL );
		m_pRenderTarget->SetDiffuseMap( m_pRenderTargets[ RENDER_DEFERRED ].GetRenderShaderResource() );

		//============================ POST PROCESSING EFFECTS ================================
		m_pRenderTargets[ RENDER_POST ].SetRenderTarget( _pDevice, 1, m_pDiffuseMRT->GetDepthStencilView() );
		m_pRenderTargets[ RENDER_POST ].ClearRenderTarget( _pDevice, m_pDiffuseMRT->GetDepthStencilView(), m_pClearColour );
		DrawScene( _pDevice, &m_pShaderCollection[ SHADERPOST_RADIALBLUR ], m_pOrthoCamera, SCENE_FINAL );
		m_pRenderTarget->SetDiffuseMap( m_pRenderTargets[ RENDER_POST ].GetRenderShaderResource() );

		//Prepare swapchain buffers
		m_pRenderer->PrepareLastScene();
		DrawScene( _pDevice, &m_pShaderCollection[ SHADERPOST_RADIALBLUR ], m_pOrthoCamera, SCENE_FINAL );
	}
	//============================ DRAW DEBUG INFORMATION ================================
	if( m_eRenderState >= RENDERSTATE_DEBUG )
	{
		_pDevice->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST );
		_pDevice->IASetInputLayout( m_pVertexLayout[ VERTEX_POINT ] );

		//============================ DEBUG LINE RENDER ================================
		DrawScene( _pDevice, &m_pShaderCollection[ SHADER_LINERENDERER ], m_pCamera, SCENE_DEBUG );
		if( m_eRenderState >= RENDERSTATE_EDITOR )
		{
			//Render UI objects to screen
			m_pResourceManager->SendTextureDataToShader( _pDevice );
			DrawScene( _pDevice, &m_pShaderCollection[ SHADER_POINTSPRITE ], m_pOrthoCamera, SCENE_UI );
		}
		//============================ FONT RENDER ================================
		_pDevice->IASetInputLayout( m_pVertexLayout[ VERTEX_FONT ] );
		DrawScene( _pDevice, &m_pShaderCollection[ SHADER_FONT ], m_pOrthoCamera, SCENE_FONT );
		_pDevice->IASetInputLayout( m_pVertexLayout[ VERTEX_POINT ] );

	}

	//Unbind all render targets and shader resources prior to next frame
	ID3D11ShaderResourceView* const blankTexture[ 3 ] = {NULL, NULL, NULL};
	_pDevice->PSSetShaderResources( 0, 3, blankTexture );
	_pDevice->OMSetRenderTargets( 0, 0, NULL );
}

void Level::DrawScene( ID3D11DeviceContext* _pDevice, Shader* _pSceneShader, Camera* _pCurrentCamera, EGameScene _EGameScene )
{
	//Set shaders for this scene
	_pDevice->VSSetShader( _pSceneShader->GetVertexShader(), NULL, 0 );
	_pDevice->GSSetShader( _pSceneShader->GetGeometryShader(), NULL, 0 );
	_pDevice->PSSetShader( _pSceneShader->GetPixelShader(), NULL, 0 );

	//Draw all entities in scene
	m_pEntityManager->Draw( _pDevice, _pCurrentCamera, _EGameScene );
}

void
Level::AddTextToSceneGraph( TEntityNode* _pEntityNode, int& _iTextOffset, int _iTabCount )
{
	std::string sText = "";
	for( int iSpaceCount = 0; iSpaceCount < _iTabCount; ++iSpaceCount )
	{
		sText += "--";
	}
	sText += _pEntityNode->pEntity->GetEntityType();
	m_pFont[ FONT_SCENEGRAPH ].Write( sText, _iTextOffset );
	for( unsigned int iChild = 0; iChild < _pEntityNode->vecChildren.size(); ++iChild )
	{
		++_iTextOffset;
		AddTextToSceneGraph( _pEntityNode->vecChildren[ iChild ], _iTextOffset, _iTabCount + 1 );
	}
}

void
Level::CreateRenderTargets( ID3D11Device* _pDevice )
{
	m_pRenderTargets = new RenderToTexture[ RENDER_MAX ];
	m_pRenderTargets[ RENDER_MRT ].Initialise( _pDevice, true, false, static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ) );
	m_pRenderTargets[ RENDER_DEFERRED ].Initialise( _pDevice, true, false, static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ) );
	m_pRenderTargets[ RENDER_POST ].Initialise( _pDevice, true, false, static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ) );
	m_pRenderTargets[ RENDER_FINAL ].Initialise( _pDevice, true, false, static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ) );

	m_pDiffuseMRT = new RenderToTexture();
	m_pDiffuseMRT->Initialise( _pDevice, true, false, static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ) );
	m_pNormalsMRT = new RenderToTexture();
	m_pNormalsMRT->Initialise( _pDevice, true, false, static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ) );
	m_pPositionMRT = new RenderToTexture();
	m_pPositionMRT->Initialise( _pDevice, true, false, static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ) );
	m_pDepthMRT = new RenderToTexture();
	m_pDepthMRT->Initialise( _pDevice, true, false, static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ) );
	m_pMRT = new ID3D11RenderTargetView*[ 4 ];
	m_pMRT[ 0 ] = m_pDiffuseMRT->GetRenderTarget();
	m_pMRT[ 1 ] = m_pNormalsMRT->GetRenderTarget();
	m_pMRT[ 2 ] = m_pPositionMRT->GetRenderTarget();
	m_pMRT[ 3 ] = m_pDepthMRT->GetRenderTarget();
}

void
Level::LoadShaderData( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext )
{
	m_pShaderCollection = new Shader[ SHADER_MAX ];
	m_pShaderCollection[ SHADER_POINTSPRITE ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_POINTSPRITE ].CompileVertexShader( _pDevice, L"Assets/Shaders/pointsprite_vs.hlsl", "PointVS" );
	m_pShaderCollection[ SHADER_POINTSPRITE ].CompileGeometryShader( _pDevice, L"Assets/Shaders/pointsprite_gs.hlsl", "PointGS" );
	m_pShaderCollection[ SHADER_POINTSPRITE ].CompilePixelShader( _pDevice, L"Assets/Shaders/pointsprite_ps.hlsl", "PointPS" );

	m_pShaderCollection[ SHADER_LINERENDERER ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_LINERENDERER ].CompileVertexShader( _pDevice, L"Assets/Shaders/linerenderer_vs.hlsl", "LineVS" );
	m_pShaderCollection[ SHADER_LINERENDERER ].CompileGeometryShader( _pDevice, L"Assets/Shaders/linerenderer_gs.hlsl", "LineGS" );
	m_pShaderCollection[ SHADER_LINERENDERER ].CompilePixelShader( _pDevice, L"Assets/Shaders/linerenderer_ps.hlsl", "LinePS" );

	m_pShaderCollection[ SHADER_OBJECT ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_OBJECT ].CompileVertexShader( _pDevice, L"Assets/Shaders/objectshader_vs.hlsl", "ObjectVS" );
	m_pShaderCollection[ SHADER_OBJECT ].CompilePixelShader( _pDevice, L"Assets/Shaders/objectshader_ps.hlsl", "ObjectPS" );

	m_pShaderCollection[ SHADER_UNLITOBJECT ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_UNLITOBJECT ].CompileVertexShader( _pDevice, L"Assets/Shaders/objectshader_vs.hlsl", "ObjectVS" );
	m_pShaderCollection[ SHADER_UNLITOBJECT ].CompilePixelShader( _pDevice, L"Assets/Shaders/objectshader_ps.hlsl", "UnlitObjectPS" );

	m_pShaderCollection[ SHADER_ANIMOBJECT ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_ANIMOBJECT ].CompileVertexShader( _pDevice, L"Assets/Shaders/animatedobject_vs.hlsl", "AnimVS" );
	m_pShaderCollection[ SHADER_ANIMOBJECT ].CompilePixelShader( _pDevice, L"Assets/Shaders/objectshader_ps.hlsl", "ObjectPS" );

	m_pShaderCollection[ SHADER_MRT ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_MRT ].CompileVertexShader( _pDevice, L"Assets/Shaders/objectshader_vs.hlsl", "ObjectVS" );
	m_pShaderCollection[ SHADER_MRT ].CompilePixelShader( _pDevice, L"Assets/Shaders/mrtshader_ps.hlsl", "MRTPS" );

	m_pShaderCollection[ SHADER_GRASS ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_GRASS ].CompileVertexShader( _pDevice, L"Assets/Shaders/grassshader_vs.hlsl", "GrassVS" );
	m_pShaderCollection[ SHADER_GRASS ].CompileGeometryShader( _pDevice, L"Assets/Shaders/grassshader_gs.hlsl", "GrassGS" );
	m_pShaderCollection[ SHADER_GRASS ].CompilePixelShader( _pDevice, L"Assets/Shaders/grassshader_ps.hlsl", "GrassMRTPS" );

	m_pShaderCollection[ SHADER_DEFERRED ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_DEFERRED ].CompileVertexShader( _pDevice, L"Assets/Shaders/objectshader_vs.hlsl", "ObjectVS" );
	m_pShaderCollection[ SHADER_DEFERRED ].CompilePixelShader( _pDevice, L"Assets/Shaders/deferred_ps.hlsl", "DeferredPS" );

	m_pShaderCollection[ SHADER_FINALOUTPUT ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_FINALOUTPUT ].CompileVertexShader( _pDevice, L"Assets/Shaders/objectshader_vs.hlsl", "ObjectVS" );
	m_pShaderCollection[ SHADER_FINALOUTPUT ].CompilePixelShader( _pDevice, L"Assets/Shaders/postprocessing.hlsl", "FinalColourPS" );

	m_pShaderCollection[ SHADERPOST_PIXELATE ].Initialise( _pDevice );
	m_pShaderCollection[ SHADERPOST_PIXELATE ].CompileVertexShader( _pDevice, L"Assets/Shaders/objectshader_vs.hlsl", "ObjectVS" );
	m_pShaderCollection[ SHADERPOST_PIXELATE ].CompilePixelShader( _pDevice, L"Assets/Shaders/postprocessing.hlsl", "PixelatePS" );

	m_pShaderCollection[ SHADERPOST_RADIALBLUR ].Initialise( _pDevice );
	m_pShaderCollection[ SHADERPOST_RADIALBLUR ].CompileVertexShader( _pDevice, L"Assets/Shaders/objectshader_vs.hlsl", "ObjectVS" );
	m_pShaderCollection[ SHADERPOST_RADIALBLUR ].CompilePixelShader( _pDevice, L"Assets/Shaders/postprocessing.hlsl", "RadialBlurPS" );

	m_pShaderCollection[ SHADER_FONT ].Initialise( _pDevice );
	m_pShaderCollection[ SHADER_FONT ].CompileVertexShader( _pDevice, L"Assets/Shaders/fontshader_vs.hlsl", "FontVS" );
	m_pShaderCollection[ SHADER_FONT ].CompileGeometryShader( _pDevice, L"Assets/Shaders/fontshader_gs.hlsl", "FontGS" );
	m_pShaderCollection[ SHADER_FONT ].CompilePixelShader( _pDevice, L"Assets/Shaders/fontshader_ps.hlsl", "FontPS" );
}

void
Level::BuildLevelVertexLayouts( ID3D11Device* _pDevice, ID3D11DeviceContext* _pDevContext )
{
	m_pVertexLayout = new ID3D11InputLayout*[ VERTEX_MAX ];
	for( int iLayout = 0; iLayout < VERTEX_MAX; ++iLayout )
	{
		m_pVertexLayout[ iLayout ] = 0;
	}

	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0,    DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0,   DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	int iNumElements = 6;

	// Create the input layout
	HRCheck( _pDevice->CreateInputLayout( vertexDesc,
		iNumElements,
		m_pShaderCollection[ SHADER_OBJECT ].GetShaderBlob()->GetBufferPointer(),
		m_pShaderCollection[ SHADER_OBJECT ].GetShaderBlob()->GetBufferSize(),
		&m_pVertexLayout[ VERTEX_STATIC ] ),
		L"Could not create static input layout" );
	// Create the animated vertex input layout.
	D3D11_INPUT_ELEMENT_DESC animDesc[] =
	{
		{ "POSITION", 0,   DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0,      DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0,     DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BINORMAL", 0,   DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0,   DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEID", 0,     DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONEWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	iNumElements = 8;

	// Create the input layout
	HRCheck( _pDevice->CreateInputLayout( animDesc,
		iNumElements,
		m_pShaderCollection[ SHADER_ANIMOBJECT ].GetShaderBlob()->GetBufferPointer(),
		m_pShaderCollection[ SHADER_ANIMOBJECT ].GetShaderBlob()->GetBufferSize(),
		&m_pVertexLayout[ VERTEX_ANIMATED ] ),
		L"Could not create animated input layout" );

	D3D11_INPUT_ELEMENT_DESC pointDesc[] =
	{
		{ "POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "DIRECTION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SCALE", 0,     DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0,     DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "ROTATION", 0,  DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXID", 0,     DXGI_FORMAT_R32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	iNumElements = 6;

	// Create the input layout
	HRCheck( _pDevice->CreateInputLayout( pointDesc,
		iNumElements,
		m_pShaderCollection[ SHADER_POINTSPRITE ].GetShaderBlob()->GetBufferPointer(),
		m_pShaderCollection[ SHADER_POINTSPRITE ].GetShaderBlob()->GetBufferSize(),
		&m_pVertexLayout[ VERTEX_POINT ] ),
		L"Could not create point sprite input layout" );

	D3D11_INPUT_ELEMENT_DESC fontDesc[] =
	{
		{ "POSITION", 0,      DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "SCALE", 0,         DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0,         DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UVTOPLEFT", 0,     DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UVBOTTOMRIGHT", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	iNumElements = 5;

	// Create the input layout
	HRCheck( _pDevice->CreateInputLayout( fontDesc,
		iNumElements,
		m_pShaderCollection[ SHADER_FONT ].GetShaderBlob()->GetBufferPointer(),
		m_pShaderCollection[ SHADER_FONT ].GetShaderBlob()->GetBufferSize(), //CHANGE THIS!!!
		&m_pVertexLayout[ VERTEX_FONT ] ),
		L"Could not create font input layout" );

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
	HRCheck( _pDevice->CreateRasterizerState( &tRasterDesc, &m_pRasteriserState ),
		L"Could not create rasteriser state" );

	tRasterDesc.CullMode = D3D11_CULL_NONE;
	HRCheck( _pDevice->CreateRasterizerState( &tRasterDesc, &m_pGrassRasteriser ),
		L"Could not create grass rasteriser state" );

	_pDevContext->RSSetState( m_pRasteriserState );

	//=================  CREATE TEXTURE SAMPLER STATE ====================
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[ 0 ] = 0;
	samplerDesc.BorderColor[ 1 ] = 0;
	samplerDesc.BorderColor[ 2 ] = 0;
	samplerDesc.BorderColor[ 3 ] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HRCheck( _pDevice->CreateSamplerState( &samplerDesc, &m_pSamplerState ),
		L"Failed to create sampler state" );
}

void
Level::OnResize( int _iWidth, int _iHeight )
{
	float fAspectRatio = static_cast<float>( _iWidth ) / static_cast<float>( _iHeight );
	if( m_pCamera )
	{
		m_pCamera->CreateProjectionMatrix( fAspectRatio );
		m_pOrthoCamera->CreateProjectionMatrix( fAspectRatio );
	}
}

Prefab*
Level::CreateObject( ID3D11Device* _pDevice, rapidxml::xml_node<>* _pNode, TEntityNode* _pParentNode )
{
	//Get prefab type
	std::string sType = _pNode->first_node( "type" )->value();

	//Get Position Scale and Rotation data
	Math::Vector3 vecPosition( ReadFromString<float>( _pNode->first_node( "position" )->first_attribute( "x" )->value() ),
		ReadFromString<float>( _pNode->first_node( "position" )->first_attribute( "y" )->value() ),
		ReadFromString<float>( _pNode->first_node( "position" )->first_attribute( "z" )->value() ) );
	Math::Vector3 vecScale( ReadFromString<float>( _pNode->first_node( "scale" )->first_attribute( "x" )->value() ),
		ReadFromString<float>( _pNode->first_node( "scale" )->first_attribute( "y" )->value() ),
		ReadFromString<float>( _pNode->first_node( "scale" )->first_attribute( "z" )->value() ) );
	Math::Vector3 vecRotation( ReadFromString<float>( _pNode->first_node( "rotation" )->first_attribute( "x" )->value() ),
		ReadFromString<float>( _pNode->first_node( "rotation" )->first_attribute( "y" )->value() ),
		ReadFromString<float>( _pNode->first_node( "rotation" )->first_attribute( "z" )->value() ) );
	//Get Prefab colour							  
	Math::Colour prefabColour( ReadFromString<float>( _pNode->first_node( "colour" )->first_attribute( "r" )->value() ),
		ReadFromString<float>( _pNode->first_node( "colour" )->first_attribute( "g" )->value() ),
		ReadFromString<float>( _pNode->first_node( "colour" )->first_attribute( "b" )->value() ),
		ReadFromString<float>( _pNode->first_node( "colour" )->first_attribute( "a" )->value() ) );

	//Create an instance of this prefab
	Prefab* pNewPrefab = m_pEntityManager->InstantiatePrefab( _pDevice,
		_pParentNode,
		sType,
		&m_pShaderCollection[ SHADER_MRT ],
		m_vecGrassEntities,
		SCENE_3DSCENE,
		vecPosition,
		vecScale,
		vecRotation,
		prefabColour );

	//Check if this object has children
	if( _pNode->first_node( "child" ) )
	{
		for( rapidxml::xml_node<>* pCurrentChild = _pNode->first_node( "child" ); pCurrentChild; pCurrentChild = pCurrentChild->next_sibling( "child" ) )
		{
			Prefab* pNewChild = CreateObject( _pDevice, pCurrentChild, pNewPrefab->GetNode() );
		}
	}
	//Check if this object has any lights attached
	if( _pNode->first_node( "light" ) )
	{
		for( rapidxml::xml_node<>* pCurrentLight = _pNode->first_node( "light" ); pCurrentLight; pCurrentLight = pCurrentLight->next_sibling( "light" ) )
		{
			pNewPrefab->GetNode()->vecLights.push_back( m_pLightManager->AddPoint( Math::Vector3( 0.0f, 0.1f, 0.0f ), Math::Colour( 0.3f, 0.3f, 0.7f, 1.0f ), Math::Vector3( 0.5f, 0.2f, 0.2f ), 12.0f ) );
		}
	}

	//Add new object to the level
	m_pLevelEntities.push_back( pNewPrefab );
	m_pEntityManager->AddEntity( pNewPrefab, SCENE_3DSCENE );

	return pNewPrefab;
}

Prefab*
Level::CreateObject( ID3D11Device* _pDevice, TSceneNode* _pNode, TEntityNode* _pParentNode )
{
	//Get prefab type
	std::string sType = _pNode->tEntity.sPrefabName;

	//Get Position Scale and Rotation data
	Math::Vector3 vecPosition( _pNode->tEntity.vecPosition[ 0 ],
		_pNode->tEntity.vecPosition[ 1 ],
		_pNode->tEntity.vecPosition[ 2 ] );
	Math::Vector3 vecScale( _pNode->tEntity.vecScale[ 0 ],
		_pNode->tEntity.vecScale[ 1 ],
		_pNode->tEntity.vecScale[ 2 ] );
	Math::Vector3 vecRotation( _pNode->tEntity.vecRotation[ 0 ],
		_pNode->tEntity.vecRotation[ 1 ],
		_pNode->tEntity.vecRotation[ 2 ] );
	//Get Prefab colour							  
	Math::Colour prefabColour( _pNode->tEntity.colour[ 0 ],
		_pNode->tEntity.colour[ 1 ],
		_pNode->tEntity.colour[ 2 ],
		_pNode->tEntity.colour[ 3 ] );

	//Create an instance of this prefab
	Prefab* pNewPrefab = m_pEntityManager->InstantiatePrefab( _pDevice,
		_pParentNode,
		sType,
		&m_pShaderCollection[ SHADER_MRT ],
		m_vecGrassEntities,
		SCENE_3DSCENE,
		vecPosition,
		vecScale,
		vecRotation,
		prefabColour );

	TPrefabOptions* pPrefabOptions = m_pEntityManager->GetPrefabOptions( sType );
	//Check if this object has children
	if( pPrefabOptions->vecChildren.size() > 0 )
	{
		for( unsigned int iChild = 0; iChild < pPrefabOptions->vecChildren.size(); ++iChild )
		{
			Prefab* pNewChild = CreateObject( _pDevice, pPrefabOptions->vecChildren[ iChild ], pNewPrefab->GetNode() );
		}
	}
	//Check if this object has any lights attached
	//if (pPrefabOptions->vecLights.size() > 0)
	//{
	//	for (unsigned int iLight = 0; iLight < pPrefabOptions->vecLights.size(); ++iLight)
	//	{
	//		pNewPrefab->GetNode()->vecLights.push_back(m_pLightManager->AddPoint(Math::Vector3(0.0f, 0.1f, 0.0f), Math::Colour(0.3f, 0.3f, 0.7f, 1.0f), Math::Vector3(10.5f, 0.5f, 0.2f), 1.0f));
	//	}
	//}

	//Add new object to the level
	m_pLevelEntities.push_back( pNewPrefab );
	m_pEntityManager->AddEntity( pNewPrefab, SCENE_3DSCENE );

	return pNewPrefab;
}

void
Level::LoadLevel( ID3D11Device* _pDevice, char* _pcLevelFilename )
{
	m_pEntityManager->ClearScene( SCENE_3DSCENE );
	m_pHivemind->ClearHivemind();
	m_pLightManager->DestroyLights();
	m_vecGrassEntities.clear();
	m_pSelectedObject = 0;
	m_bHasSelectedObject = false;

	m_bCreateObject = false;

	m_pRootNode->Clear();
	delete m_pRootNode;

	m_pRootNode = m_pTerrain->CreateNode( NULL );

	//Delete all current entities in the scene
	for( unsigned int iEntity = 0; iEntity < m_pLevelEntities.size(); ++iEntity )
	{
		SAFEDELETE( m_pLevelEntities[ iEntity ] );
	}
	m_pLevelEntities.clear();
	//Add default point light to the scene
	m_pLightManager->AddPoint( Math::Vector3( 0.0f, 2.0f, 0.0f ), Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f ), Math::Vector3( 1.0f, 0.5f, 0.2f ), 10.0f );

	m_pSceneHierarchy->LoadSceneFromXML( _pcLevelFilename );
	//Read scene information from Scene Hierarchy
	TSceneNode* pRoot = m_pSceneHierarchy->GetRootNode();
	for( unsigned int iNode = 0; iNode < pRoot->vecChildren.size(); ++iNode )
	{
		Prefab* pNewPrefab = CreateObject( _pDevice, pRoot->vecChildren[ iNode ], m_pRootNode );
	}

	//Spawn initial testing AI
	if( m_pSetupData->bDoLog )
	{
		int iNumDefaultAI = m_pSetupData->iAICount;
		float fAIPercentage = 1.0f / static_cast<float>( iNumDefaultAI );
		float fSpawnRadius = 5.0f;
		for( int iAI = 0; iAI < iNumDefaultAI; ++iAI )
		{
			Math::Vector3 vecPosition = Math::Vector3( sinf( fAIPercentage * iAI * static_cast<float>( PI )* 2.0f ) * fSpawnRadius, 0.0f, cosf( fAIPercentage * iAI * static_cast<float>( PI )* 2.0f ) * fSpawnRadius );
			Prefab* pNewPrefab = m_pEntityManager->InstantiatePrefab( _pDevice,
				m_pRootNode,
				std::string( "chicken" ),
				&m_pShaderCollection[ SHADER_MRT ],
				m_vecGrassEntities,
				SCENE_3DSCENE,
				vecPosition,
				Math::Vector3( 1.0f, 1.0f, 1.0f ),
				Math::Vector3( 0.0f, 0.0f, 0.0f ),
				Math::Colour( 1.0f, 1.0f, 1.0f, 1.0f ) );

			//Add new object to the level
			m_pLevelEntities.push_back( pNewPrefab );
			m_pEntityManager->AddEntity( pNewPrefab, SCENE_3DSCENE );
		}
	}
}

void
Level::SaveLevel( ID3D11Device* _pDevice, char* _pcLevelFilename )
{
	rapidxml::xml_document<> xmlDoc;
	//Add xml header
	rapidxml::xml_node<>* pDeclaration = xmlDoc.allocate_node( rapidxml::node_declaration );
	pDeclaration->append_attribute( xmlDoc.allocate_attribute( "version", "1.0" ) );
	pDeclaration->append_attribute( xmlDoc.allocate_attribute( "encoding", "utf-8" ) );
	xmlDoc.append_node( pDeclaration );

	//Create root node
	rapidxml::xml_node<>* pRoot = xmlDoc.allocate_node( rapidxml::node_element, "level" );
	xmlDoc.append_node( pRoot );
	//Create camera node
	rapidxml::xml_node<>* pCameraNode = xmlDoc.allocate_node( rapidxml::node_element, "camera" );
	//Camera position
	char cBuffer[ 64 ];
	rapidxml::xml_node<>* pPosition = xmlDoc.allocate_node( rapidxml::node_element, "position" );
	sprintf_s( cBuffer, 32, "%.2f", m_pCamera->GetPosition().x );
	pPosition->append_attribute( xmlDoc.allocate_attribute( "x", xmlDoc.allocate_string( cBuffer ) ) );
	sprintf_s( cBuffer, 32, "%.2f", m_pCamera->GetPosition().y ); // Lower the model by half its scale
	pPosition->append_attribute( xmlDoc.allocate_attribute( "y", xmlDoc.allocate_string( cBuffer ) ) );
	sprintf_s( cBuffer, 32, "%.2f", m_pCamera->GetPosition().z );
	pPosition->append_attribute( xmlDoc.allocate_attribute( "z", xmlDoc.allocate_string( cBuffer ) ) );
	//Camera direction
	rapidxml::xml_node<>* pDirection = xmlDoc.allocate_node( rapidxml::node_element, "direction" );
	sprintf_s( cBuffer, 32, "%.2f", m_pCamera->GetLook().x );
	pDirection->append_attribute( xmlDoc.allocate_attribute( "x", xmlDoc.allocate_string( cBuffer ) ) );
	sprintf_s( cBuffer, 32, "%.2f", m_pCamera->GetLook().y ); // Lower the model by half its scale
	pDirection->append_attribute( xmlDoc.allocate_attribute( "y", xmlDoc.allocate_string( cBuffer ) ) );
	sprintf_s( cBuffer, 32, "%.2f", m_pCamera->GetLook().z );
	pDirection->append_attribute( xmlDoc.allocate_attribute( "z", xmlDoc.allocate_string( cBuffer ) ) );
	pCameraNode->append_node( pPosition );
	pCameraNode->append_node( pDirection );
	pRoot->append_node( pCameraNode );

	//Loop through all entities attached to the root node
	for( unsigned int iChild = 0; iChild < m_pRootNode->vecChildren.size(); ++iChild )
	{
		AddChildToXMLNode( &xmlDoc, pRoot, m_pRootNode->vecChildren[ iChild ] );
	}

	//Write this document data to a string
	std::string sXMLString;
	rapidxml::print( std::back_inserter( sXMLString ), xmlDoc );

	//Save this string to file
	std::ofstream outFile;
	outFile.open( _pcLevelFilename );

	outFile << xmlDoc;
	outFile.close();
}

void
Level::AddChildToXMLNode( rapidxml::xml_document<>* _pDocument, rapidxml::xml_node<>* _pParentNode, TEntityNode* _pChildNode )
{
	char cBuffer[ 32 ];
	RenderEntity* pCurrentEntity = _pChildNode->pEntity;
	if( pCurrentEntity->DoDraw() )
	{
		rapidxml::xml_node<>* pChild = _pDocument->allocate_node( rapidxml::node_element, "child" );
		//Object type
		pChild->append_node( _pDocument->allocate_node( rapidxml::node_element, "type", _pChildNode->pEntity->GetEntityType().c_str() ) );
		//Object position
		rapidxml::xml_node<>* pPosition = _pDocument->allocate_node( rapidxml::node_element, "position" );
		sprintf_s( cBuffer, 32, "%.2f", pCurrentEntity->GetPosition().x );
		pPosition->append_attribute( _pDocument->allocate_attribute( "x", _pDocument->allocate_string( cBuffer ) ) );
		sprintf_s( cBuffer, 32, "%.2f", pCurrentEntity->GetPosition().y ); // Lower the model by half its scale
		pPosition->append_attribute( _pDocument->allocate_attribute( "y", _pDocument->allocate_string( cBuffer ) ) );
		sprintf_s( cBuffer, 32, "%.2f", pCurrentEntity->GetPosition().z );
		pPosition->append_attribute( _pDocument->allocate_attribute( "z", _pDocument->allocate_string( cBuffer ) ) );
		pChild->append_node( pPosition );
		//Object scale
		rapidxml::xml_node<>* pScale = _pDocument->allocate_node( rapidxml::node_element, "scale" );
		sprintf_s( cBuffer, 32, "%.2f", pCurrentEntity->GetScale().x / m_pEntityManager->GetPrefabOptions( pCurrentEntity->GetEntityType() )->vecScale.x );
		pScale->append_attribute( _pDocument->allocate_attribute( "x", _pDocument->allocate_string( cBuffer ) ) );
		sprintf_s( cBuffer, 32, "%.2f", pCurrentEntity->GetScale().y / m_pEntityManager->GetPrefabOptions( pCurrentEntity->GetEntityType() )->vecScale.y );
		pScale->append_attribute( _pDocument->allocate_attribute( "y", _pDocument->allocate_string( cBuffer ) ) );
		sprintf_s( cBuffer, 32, "%.2f", pCurrentEntity->GetScale().z / m_pEntityManager->GetPrefabOptions( pCurrentEntity->GetEntityType() )->vecScale.z );
		pScale->append_attribute( _pDocument->allocate_attribute( "z", _pDocument->allocate_string( cBuffer ) ) );
		pChild->append_node( pScale );
		//Object rotation
		rapidxml::xml_node<>* pRotation = _pDocument->allocate_node( rapidxml::node_element, "rotation" );
		sprintf_s( cBuffer, 32, "%.2f", pCurrentEntity->GetRotation().x );
		pRotation->append_attribute( _pDocument->allocate_attribute( "x", _pDocument->allocate_string( cBuffer ) ) );
		sprintf_s( cBuffer, 32, "%.2f", pCurrentEntity->GetRotation().y );
		pRotation->append_attribute( _pDocument->allocate_attribute( "y", _pDocument->allocate_string( cBuffer ) ) );
		sprintf_s( cBuffer, 32, "%.2f", pCurrentEntity->GetRotation().z );
		pRotation->append_attribute( _pDocument->allocate_attribute( "z", _pDocument->allocate_string( cBuffer ) ) );
		pChild->append_node( pRotation );
		//Object colour
		rapidxml::xml_node<>* pColour = _pDocument->allocate_node( rapidxml::node_element, "colour" );
		pColour->append_attribute( _pDocument->allocate_attribute( "r", _pDocument->allocate_string( "1.0f" ) ) );
		pColour->append_attribute( _pDocument->allocate_attribute( "g", _pDocument->allocate_string( "1.0f" ) ) );
		pColour->append_attribute( _pDocument->allocate_attribute( "b", _pDocument->allocate_string( "1.0f" ) ) );
		pColour->append_attribute( _pDocument->allocate_attribute( "a", _pDocument->allocate_string( "1.0f" ) ) );
		pChild->append_node( pColour );

		//Do the same for all children of this class
		for( unsigned int iChild = 0; iChild < _pChildNode->vecChildren.size(); ++iChild )
		{
			AddChildToXMLNode( _pDocument, pChild, _pChildNode->vecChildren[ iChild ] );
		}
		//Add lights attached to this object to file
		for( unsigned int iLight = 0; iLight < _pChildNode->vecLights.size(); ++iLight )
		{
			rapidxml::xml_node<>* pLight = _pDocument->allocate_node( rapidxml::node_element, "light" );
			pLight->append_node( _pDocument->allocate_node( rapidxml::node_element, "type", "point" ) );
			pChild->append_node( pLight );
		}

		_pParentNode->append_node( pChild );
	}
}

void
Level::ChangeGrassProcessingMethod( EProcessingMethod _eProcessingMethod )
{
	m_eGrassProcessingMethod = _eProcessingMethod;
}

void
Level::ChangeAIProcessingMethod( EProcessingMethod _eProcessingMethod )
{
	m_eAIProcessingMethod = _eProcessingMethod;
	m_pHivemind->ChangeProcessingMethod( _eProcessingMethod );
}