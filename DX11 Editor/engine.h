
#pragma once

// WINDOWS DEFINES
#define WIN32_LEAN_AND_MEAN

// Includes
#include <Windows.h>

// Local includes
#include "inputmanager.h"

class IRenderer;

class Engine
{
public:
	Engine();
	~Engine();

	bool Initialise();
	void Run();
	LRESULT CALLBACK MessageHandler( HWND _hWnd, UINT _iMsg, WPARAM _wParam, LPARAM _lParam );

private:
	void ExecuteOneFrame();
	void Shutdown();

	void InitialiseWindows( const int _iWidth, const int _iHeight );
	void ShutdownWindows();

	wchar_t* m_pApplicationName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	IRenderer* m_pRenderer;
	InputManager* m_inputManager;

	bool m_bFullscreen;
	int m_iWindowWidth;
	int m_iWindowHeight;

};

// Static
static LRESULT CALLBACK WndProc( HWND _hWnd, UINT _iMsg, WPARAM _wParam, LPARAM _lParam );
static Engine* s_EngineHandle = 0;