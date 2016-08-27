
#pragma once

// Local includes
#include "inputmanager.h"
#include "datastructures.h"

class IRenderer;
class DX11Renderer;
class Clock;
class Level;
class ConsoleWindow;

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

	virtual void ReadProgramSetupFile(char* _pcFilename);

	wchar_t* m_pApplicationName;
	HINSTANCE m_hInstance;
	HWND m_hWnd;

	DX11Renderer* m_pRenderer;
	Level* m_pLevel;
	InputManager* m_inputManager;

	ConsoleWindow* m_pConsoleWindow;
	TSetupStruct* m_pSetupData;
	Clock* m_pClock;
	
	bool m_bFullscreen;
	int m_iWindowWidth;
	int m_iWindowHeight;

};

// Static
static LRESULT CALLBACK WndProc( HWND _hWnd, UINT _iMsg, WPARAM _wParam, LPARAM _lParam );
static Engine* s_EngineHandle = 0;