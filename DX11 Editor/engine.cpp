
// Library Includes
#include <iostream>
#include <rapidxml_utils.hpp>

// This include
#include "engine.h"

// Local Includes
#include "defines.h"
#include "inputmanager.h"
#include "clock.h"
#include "level.h"
#include "consolewindow.h"
#ifdef PLANET_DX11
#	include "dx11renderer.h"
#endif


Engine::Engine()
	: m_inputManager( InputManager::GetInstance() )
	, m_pRenderer( nullptr )
	, m_bFullscreen( false )
	, m_pApplicationName( L"Planet" )
{
}

Engine::~Engine()
{
	Shutdown();
}

bool Engine::Initialise()
{
	//Setup game clock
	m_pClock = new Clock();
	m_pClock->Initialise();
	m_pClock->LimitFramesPerSecond( 60.0f );

	m_pConsoleWindow = new ConsoleWindow();
	m_pConsoleWindow->InitialiseConsole();

	ReadProgramSetupFile( "Data/Setup.xml" );

	InitialiseWindows( static_cast<int>( WINDOW_WIDTH ), static_cast<int>( WINDOW_HEIGHT ) );

	m_pRenderer = new DX11Renderer();
	Assert( m_pRenderer->Initialise( m_hWnd, nullptr, m_iWindowWidth, m_iWindowHeight ), L"Renderer failed to initialise" );

	m_pLevel = new Level();
	m_pLevel->Initialise( static_cast< DX11Renderer* >( m_pRenderer ), m_pSetupData, m_hWnd, m_iWindowWidth, m_iWindowHeight );

	return true;
}

void Engine::Run()
{
	MSG msg;
	bool is_running = true;

	ZeroMemory( &msg, sizeof( MSG ) );

	while( is_running )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		if( msg.message == WM_QUIT )
			is_running = false;
		else
			ExecuteOneFrame();
	}
}

void Engine::ExecuteOneFrame()
{
	m_pClock->Process();
	float fTimeElapsed = m_pClock->GetDeltaTick();
	m_pRenderer->SetFPSCount( m_pClock->GetFPS() );

	m_pLevel->Process( m_pRenderer->GetDevice(), m_pRenderer->GetDeviceContext(), m_pClock, fTimeElapsed );
	m_pLevel->Draw( m_pRenderer->GetDeviceContext() );
	
	m_pRenderer->ExecuteOneFrame( m_pClock, fTimeElapsed );
}

void Engine::Shutdown()
{
	assert( m_pRenderer );
	m_pRenderer->CleanUp();
	SAFEDELETE( m_pRenderer );
}

LRESULT CALLBACK Engine::MessageHandler( HWND _hWnd, UINT _iMsg, WPARAM _wParam, LPARAM _lParam )
{
	assert( m_inputManager );

	switch( _iMsg )
	{
	case WM_DESTROY:
	case WM_CLOSE:
	{
		PostQuitMessage( 0 );
		return 0;
	}
	default:
		const bool input_used = m_inputManager->ProcessInput( _hWnd, _iMsg, _wParam, _lParam );
		return ( !input_used ) ? DefWindowProc( _hWnd, _iMsg, _wParam, _lParam ) : 0; 
	}
}

void Engine::InitialiseWindows( const int _iWidth, const int _iHeight )
{
	m_iWindowWidth = _iWidth;
	m_iWindowHeight = _iHeight;

	WNDCLASSEX winClass;
	int posX;
	int posY;

	s_EngineHandle = this;
	m_hInstance = GetModuleHandle( NULL );

	winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	winClass.lpfnWndProc = WndProc;
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hInstance = m_hInstance;
	winClass.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	winClass.hIconSm = winClass.hIcon;
	winClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	winClass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	winClass.lpszMenuName = NULL;
	winClass.lpszClassName = m_pApplicationName;
	winClass.cbSize = sizeof( WNDCLASSEX );

	RegisterClassEx( &winClass );

	if( m_bFullscreen )
	{
		DEVMODE dmScreenSettings;
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );
		dmScreenSettings.dmSize = sizeof( dmScreenSettings );
		dmScreenSettings.dmPelsWidth = (unsigned long)m_iWindowWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)m_iWindowHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN );

		posX = 0;
		posY = 0;
	}
	else
	{
		m_iWindowWidth = 1280;
		m_iWindowHeight = 720;
		posX = ( GetSystemMetrics( SM_CXSCREEN ) - m_iWindowWidth ) / 2;
		posY = ( GetSystemMetrics( SM_CYSCREEN ) - m_iWindowHeight ) / 2;
	}

	m_hWnd = CreateWindowEx( WS_EX_APPWINDOW, m_pApplicationName, m_pApplicationName, 
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, m_iWindowWidth, m_iWindowHeight, NULL, NULL, m_hInstance, NULL );

	ShowWindow( m_hWnd, SW_SHOW );
	SetForegroundWindow( m_hWnd );
	SetFocus( m_hWnd );

	return;
}

void Engine::ShutdownWindows()
{
	if( m_bFullscreen )
		ChangeDisplaySettings( NULL, 0 );

	DestroyWindow( m_hWnd );
	m_hWnd = NULL;

	UnregisterClass( m_pApplicationName, m_hInstance );
	m_hInstance = NULL;

	s_EngineHandle = nullptr;
}


void
Engine::ReadProgramSetupFile( char* _pcFilename )
{
	m_pSetupData = new TSetupStruct();

	//Open file containing setup information
	rapidxml::file<> xmlFile( _pcFilename );
	rapidxml::xml_document<> xmlDoc;

	//Parse file string
	xmlDoc.parse<0>( xmlFile.data() );
	rapidxml::xml_node<>* pRoot = xmlDoc.first_node();
	rapidxml::xml_node<>* pLevelSetup = pRoot->first_node( "level" );

	rapidxml::xml_node<>* pGeneralSetup = pLevelSetup->first_node( "general" );
	rapidxml::xml_node<>* pAISetup = pLevelSetup->first_node( "ai" );
	rapidxml::xml_node<>* pGrassSetup = pLevelSetup->first_node( "grass" );
	rapidxml::xml_node<>* pLoggingSetup = pLevelSetup->first_node( "logging" );
	rapidxml::xml_node<>* pProcessingSetup = pLevelSetup->first_node( "processing" );

	//General
	if( pGeneralSetup )
	{
		char* pcLevelFile = pGeneralSetup->first_node( "defaultlevel" )->value();
		int iStringSize = strlen( pcLevelFile ) + 1;
		m_pSetupData->pcDefaultLevel = new char[ iStringSize ];
		sprintf_s( m_pSetupData->pcDefaultLevel, iStringSize, "%s", pcLevelFile );

		m_pSetupData->eRenderState = static_cast<ERenderState>( ReadFromString<int>( pGeneralSetup->first_node( "renderstate" )->value() ) );
		m_pSetupData->bPlaySound = ( strcmp( pGeneralSetup->first_node( "playsound" )->value(), "true" ) == 0 );
	}
	//AI
	if( pAISetup )
	{
		m_pSetupData->iAICount = ReadFromString<int>( pAISetup->first_node( "number" )->value() );
		m_pSetupData->iAStarSearchDepth = ReadFromString<int>( pAISetup->first_node( "astardepth" )->value() );
	}
	//Grass
	if( pGrassSetup )
	{
		m_pSetupData->iGrassDimensions = ReadFromString<int>( pGrassSetup->first_node( "dimensions" )->value() );
		m_pSetupData->eGrassState = static_cast<EGrassState>( ReadFromString<int>( pGrassSetup->first_node( "state" )->value() ) );
	}
	//Logging
	if( pLoggingSetup )
	{
		m_pSetupData->iLogFrameDuration = ReadFromString<int>( pLoggingSetup->first_node( "duration" )->value() );
		m_pSetupData->iLogFrameSkip = ReadFromString<int>( pLoggingSetup->first_node( "delay" )->value() );
		m_pSetupData->bDoLog = ( strcmp( pLoggingSetup->first_node( "dolog" )->value(), "true" ) == 0 );
	}
	//Processing
	if( pProcessingSetup )
	{
		m_pSetupData->eGrassProcessing = static_cast<EProcessingMethod>( ReadFromString<int>( pProcessingSetup->first_node( "grass" )->value() ) );
		m_pSetupData->eAIProcessing = static_cast<EProcessingMethod>( ReadFromString<int>( pProcessingSetup->first_node( "ai" )->value() ) );
	}

	//Generate description for log file
	m_pSetupData->pcLogDescription = new char[ 128 ];
	sprintf_s( m_pSetupData->pcLogDescription,
		128,
		"Logging performance over %i frames:\nGrass\n-\tDim: %i\n-\tState: %i\nProcessing\n-\tGrass: %i\n-\tAI: %i\n\n",
		m_pSetupData->iLogFrameDuration,
		m_pSetupData->iGrassDimensions,
		m_pSetupData->eGrassState,
		m_pSetupData->eGrassProcessing,
		m_pSetupData->eAIProcessing );
	//Generate filename for log file
	m_pSetupData->pcLogFilename = new char[ 128 ];
	sprintf_s( m_pSetupData->pcLogFilename,
		128,
		"Log/perflog_grass%i_AI%i_method[%i %i].xml",
		m_pSetupData->iGrassDimensions,
		m_pSetupData->iAICount,
		m_pSetupData->eGrassProcessing,
		m_pSetupData->eAIProcessing );
}

LRESULT CALLBACK WndProc( HWND _hWnd, UINT _iMsg, WPARAM _wParam, LPARAM _lParam )
{
	assert( s_EngineHandle );

	// Handle escape key press
	if( _iMsg == WM_KEYDOWN && LOWORD( _wParam ) == VK_ESCAPE )
	{
		PostQuitMessage( 0 );
		return 0;
	}

	return s_EngineHandle->MessageHandler( _hWnd, _iMsg, _wParam, _lParam );
}
