
// Library Includes
#include <iostream>
#include <rapidxml_utils.hpp>

// Local Includes
#include "defines.h"
#ifdef PLANET_DX11
#	include "dx11renderer.h"
#else
#	include "openglrenderer.h"
#endif

#include "consolewindow.h"
#include "clock.h"
#include "datastructures.h"

// This Include
#include "window.h"

// Static Variables

// Static Function Prototypes

// Implementation
#define WM_WINDOW_CREATION (WM_USER + 1)
/**
*
* Main window message handler proc
*
* @author Christopher Howlett
*
*/
LRESULT CALLBACK
WindowProc( HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam )
{
	static Window* pThisWindow = 0;

	switch( _uiMsg )
	{
	case WM_CREATE:
	{

	}
	break;
	case WM_WINDOW_CREATION:
	{
		pThisWindow = (Window*)_wParam;
	}
	break;
	case WM_KEYDOWN:
	{
		switch( LOWORD( _wParam ) )
		{
		case VK_ESCAPE:
		{
			PostQuitMessage( 0 );
		}
		break;
		}
	}
	break;
	case WM_QUIT: //Fall through
	case WM_CLOSE://Fall through
	case WM_DESTROY:
	{
		PostQuitMessage( 0 );
	}
	break;
	default:
		break;
	}

	// Don't start processing messages until after WM_CREATE.
	if( pThisWindow )
	{
		return pThisWindow->msgProc( _uiMsg, _wParam, _lParam );
	}
	else
	{
		return DefWindowProc( _hWnd, _uiMsg, _wParam, _lParam );
	}
}
/**
*
* Window Constructor
*
* @author Christopher Howlett
*
*/
Window::Window()
	: m_pRenderer( 0 )
	, m_hMainWnd( 0 )
	, mMinimized( false )
	, mMaximized( false )
	, mResizing( false )
	, m_pClock( 0 )
	, m_pConsoleWindow( 0 )
{

}
/**
*
* Window Destructor
*
* @author Christopher Howlett
*
*/
Window::~Window()
{
	if( m_pConsoleWindow )
	{
		m_pConsoleWindow->ShutdownConsole();

		delete m_pConsoleWindow;
		m_pConsoleWindow = 0;
	}
	if( m_pRenderer )
	{
		delete m_pRenderer;
		m_pRenderer = 0;
	}
	if( m_pClock )
	{
		delete m_pClock;
		m_pClock = 0;
	}
}
/**
*
* Window GetInstance
*
* @author Christopher Howlett
* @return Returns this window instance
*
*/
HINSTANCE
Window::GetInstance()
{
	return m_hInstance;
}
/**
*
* Window GetWindowHandle
*
* @author Christopher Howlett
* @return Returns this window handle
*
*/
HWND
Window::GetWindowHandle()
{
	return m_hMainWnd;
}
/**
*
* Window initialisation
*
* @author Christopher Howlett
* @param _hInstance Handle to this instance
* @param _eRenderer DirectX or OpenGL Renderer
* @return Returns success of window creation
*
*/
bool
Window::Initialise( HINSTANCE _hInstance, ERendererType _eRenderer )
{
	//Setup game clock
	m_pClock = new Clock();
	m_pClock->Initialise();
	m_pClock->LimitFramesPerSecond( 60.0f );

	m_pConsoleWindow = new ConsoleWindow();
	m_pConsoleWindow->InitialiseConsole();

	ReadProgramSetupFile( "Data/Setup.xml" );
	m_iClientWidth = static_cast<int>( WINDOW_WIDTH );
	m_iClientHeight = static_cast<int>( WINDOW_HEIGHT );

	m_hInstance = _hInstance;
	InitialiseMainWindow();
	SendMessage( m_hMainWnd, WM_WINDOW_CREATION, ( WPARAM )this, NULL );

	//Create renderer type specified
	switch( _eRenderer )
	{
	case RENDERER_OPENGL:
	{
		assert( false && "Not supported yet" );//m_pRenderer = new COpenGLRenderer();
	}
	break;
	case RENDERER_DIRECTX11:
	{
		m_pRenderer = new DX11Renderer();
	}
	break;
	};

	m_pRenderer->Initialise( m_hMainWnd, m_pSetupData, m_iClientWidth, m_iClientHeight );

	return true;
}
/**
*
* Window Run - Runs infinite loop for program
*
* @author Christopher Howlett
*
*/
void
Window::Run()
{
	MSG msg;
	ZeroMemory( &msg, sizeof( MSG ) );

	while( msg.message != WM_QUIT )
	{
		// Process windows messages
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		// Run the game
		else
		{
			ExecuteOneFrame();
		}
	}
}
/**
*
* Window Executes one frame of program
*
* @author Christopher Howlett
*
*/
void
Window::ReadProgramSetupFile( char* _pcFilename )
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
/**
*
* Window Executes one frame of program
*
* @author Christopher Howlett
*
*/
void
Window::ExecuteOneFrame()
{
	m_pClock->Process();
	float fTimeElapsed = m_pClock->GetDeltaTick();
	//m_pRenderer->SetFPSCount( m_pClock->GetFPS() );
	//m_pRenderer->ExecuteOneFrame( m_pClock, fTimeElapsed );
}
/**
*
* Window Class Message proc, receives messages from window proc
*
* @author Christopher Howlett
*
*/
LRESULT
Window::msgProc( UINT msg, WPARAM wParam, LPARAM lParam )
{
	return DefWindowProc( m_hMainWnd, msg, wParam, lParam );
}
/**
*
* Window Initialises Main Window
*
* @author Christopher Howlett
*
*/
void
Window::InitialiseMainWindow()
{
	printf( "Setting up window at %i x %i\n", WINDOW_WIDTH, WINDOW_HEIGHT );

	wchar_t cClassName[] = L"Editor Window";
	WNDCLASS winClass;

	//Create window class
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hbrBackground = (HBRUSH)GetStockObject( LTGRAY_BRUSH );
	winClass.hCursor = LoadCursor( m_hInstance, IDC_ARROW );
	winClass.hIcon = LoadIcon( m_hInstance, IDI_APPLICATION );
	winClass.hInstance = m_hInstance;
	winClass.lpfnWndProc = WindowProc;
	winClass.lpszClassName = cClassName;
	winClass.lpszMenuName = NULL;
	winClass.style = CS_VREDRAW | CS_HREDRAW;

	if( !RegisterClass( &winClass ) )
	{
		Error( L"Window creation FAILED" );
	}

	//Initialise window
	m_hMainWnd = CreateWindow( cClassName,
		L"Level Editor - Christopher Howlett",
		WS_MINIMIZEBOX | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		0,
		0,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		NULL,
		NULL,
		m_hInstance,
		NULL );
	if( !m_hMainWnd )
	{
		Error( L"Window creation failed2" );
	}
}