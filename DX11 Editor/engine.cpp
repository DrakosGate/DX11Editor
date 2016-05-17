
#include "engine.h"

#ifdef PLANET_DX11
#include "Rendering/dx11renderer.h"
#elif PLANET_OPENGL

#endif

Engine::Engine()
	: m_inputManager( InputManager::GetInstance() )
	, m_pRenderer( nullptr )
	, m_pApplicationName( L"Planet" )
	, m_bFullscreen( false )
{
}

Engine::~Engine()
{
	Shutdown();
}

bool Engine::Initialise()
{
	InitialiseWindows( GetSystemMetrics( SM_CXSCREEN ), GetSystemMetrics( SM_CYSCREEN ) );

	m_pRenderer = new DX11Renderer();
	Assert( m_pRenderer->Initialise( m_hWnd, nullptr, m_iWindowWidth, m_iWindowHeight ), L"Renderer failed to initialise" );

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
