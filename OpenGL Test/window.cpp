//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   window.cpp
//  Description :   Code for Class window app
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <iostream>

// Local Includes
#include "clock.h"
#include "openglrenderer.h"
#include "directxrenderer.h"
#include "consolewindow.h"

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
WindowProc(HWND _hWnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	static CWindow* pThisWindow = 0;

	switch( _uiMsg )
	{
		case WM_CREATE:
			{
			
			}
			break;
		case WM_WINDOW_CREATION:
			{
				pThisWindow = (CWindow*)_wParam;
			}
			break;
		case WM_KEYDOWN:
			{
				switch(LOWORD(_wParam))
				{
				case VK_ESCAPE:
					{
						PostQuitMessage(0);
					}
					break;
				}
			}
			break;
		case WM_QUIT: //Fall through
		case WM_CLOSE://Fall through
		case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			break;
		default:
			break;
	}

	// Don't start processing messages until after WM_CREATE.
	if( pThisWindow )
	{
		return pThisWindow->msgProc(_uiMsg, _wParam, _lParam);
	}
	else
	{
		return DefWindowProc(_hWnd, _uiMsg, _wParam, _lParam);
	}
}
/**
*
* CWindow Constructor
*
* @author Christopher Howlett
*
*/
CWindow::CWindow()
:	m_pRenderer(0)
,	m_hMainWnd(0)
,	mMinimized(false)
,	mMaximized(false)
,	mResizing(false)	
,	m_pClock(0)
,	m_pConsoleWindow(0)
{
	m_iClientWidth    = static_cast<int>(WINDOW_WIDTH);
	m_iClientHeight   = static_cast<int>(WINDOW_HEIGHT);
}
/**
*
* CWindow Destructor
*
* @author Christopher Howlett
*
*/
CWindow::~CWindow()
{
	if (m_pConsoleWindow)
	{
		m_pConsoleWindow->ShutdownConsole();

		delete m_pConsoleWindow;
		m_pConsoleWindow = 0;
	}
	if(m_pRenderer)
	{
		delete m_pRenderer;
		m_pRenderer = 0;
	}
	if(m_pClock)
	{
		delete m_pClock;
		m_pClock = 0;
	}
}
/**
*
* CWindow GetInstance
*
* @author Christopher Howlett
* @return Returns this window instance
*
*/
HINSTANCE 
CWindow::GetInstance()
{
	return m_hInstance;
}
/**
*
* CWindow GetWindowHandle
*
* @author Christopher Howlett
* @return Returns this window handle
*
*/
HWND 
CWindow::GetWindowHandle()
{
	return m_hMainWnd;
}
/**
*
* CWindow initialisation
*
* @author Christopher Howlett
* @param _hInstance Handle to this instance
* @param _eRenderer DirectX or OpenGL Renderer
* @return Returns success of window creation
*
*/
bool
CWindow::Initialise(HINSTANCE _hInstance, ERendererType _eRenderer)
{
	//Setup game clock
	m_pClock = new CClock();
	m_pClock->Initialise();
	m_pClock->LimitFramesPerSecond(60.0f);

	m_pConsoleWindow = new CConsoleWindow();
	m_pConsoleWindow->InitialiseConsole();

	ZeroMemory(&m_tInput, sizeof(TInputStruct));
	m_hInstance = _hInstance;
	InitialiseMainWindow();
	SendMessage(m_hMainWnd, WM_WINDOW_CREATION, (WPARAM)this, NULL);

	//Create renderer type specified
	switch(_eRenderer)
	{
	case RENDERER_OPENGL:
		{
			m_pRenderer = new COpenGLRenderer();
		}
		break;
	case RENDERER_DIRECTX11:
		{
			m_pRenderer = new CDirectXRenderer();
		}
		break;
	};

	m_pRenderer->Initialise(m_hMainWnd, m_iClientWidth, m_iClientHeight, &m_tInput);

	return true;
}
/**
*
* CWindow Run - Runs infinite loop for program
*
* @author Christopher Howlett
*
*/
void 
CWindow::Run()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
 
	while(msg.message != WM_QUIT)
	{
		// Process windows messages
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
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
* CWindow Executes one frame of program
*
* @author Christopher Howlett
*
*/
void 
CWindow::ExecuteOneFrame()
{
	m_pClock->Process();
	float fTimeElapsed = m_pClock->GetDeltaTick();
	m_pRenderer->SetFPSCount(m_pClock->GetFPS());
	m_pRenderer->ExecuteOneFrame(m_pClock, fTimeElapsed);
	//Record this frames input for the next frame
	m_tInput.RecordPreviousInput();
}
/**
*
* CWindow Class Message proc, receives messages from window proc
*
* @author Christopher Howlett
*
*/
LRESULT 
CWindow::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	m_tInput.msg.message = msg;
	m_tInput.msg.wParam = wParam;
	m_tInput.msg.lParam = lParam;
	switch (msg)
	{
		// ======================================================= 
		//				MOUSE CONTROLS
		// =======================================================  
	case WM_LBUTTONDOWN:
	{
		m_tInput.bLeftMouseClick.bPressed = true;
	}
		break;
	case WM_LBUTTONUP:
	{
		m_tInput.bLeftMouseClick.bPressed = false;
	}
		break;
	case WM_RBUTTONDOWN:
	{
		m_tInput.bRightMouseClick.bPressed = true;
	}
		break;
	case WM_RBUTTONUP:
	{
		m_tInput.bRightMouseClick.bPressed = false;
	}
		break;
	case WM_MOUSEMOVE:
	{
		m_tInput.vecMouse = D3DXVECTOR2(LOWORD(lParam) - (0.5f * m_iClientWidth), -(HIWORD(lParam) - (0.5f * m_iClientHeight)));
	}
		break;
	case WM_MOUSEWHEEL:
	{
		float fMovement = GET_WHEEL_DELTA_WPARAM(wParam);
		if (fMovement < -1.0f)
		{
			m_tInput.fMouseWheel = -1.0f;
		}
		else if (fMovement > 1.0f)
		{
			m_tInput.fMouseWheel = 1.0f;
		}
		else
		{

		}
	}
		break;
		// ======================================================= 
		//				KEYBOARD CONTROLS
		// =======================================================  
	case WM_KEYDOWN:
	{
		switch (LOWORD(wParam))
		{
		case VK_UP:
		{
			m_tInput.bUp.bPressed = true;
		}
			break;
		case VK_DOWN:
		{
			m_tInput.bDown.bPressed = true;
		}
			break;
		case VK_LEFT:
		{
			m_tInput.bLeft.bPressed = true;
		}
			break;
		case VK_RIGHT:
		{
			m_tInput.bRight.bPressed = true;
		}
			break;
		case 'W':
		{
			m_tInput.bW.bPressed = true;
		}
			break;
		case 'A':
		{
			m_tInput.bA.bPressed = true;
		}
			break;
		case 'S':
		{
			m_tInput.bS.bPressed = true;
		}
			break;
		case 'D':
		{
			m_tInput.bD.bPressed = true;
		}
			break;
		case 'G':
		{
			m_tInput.bG.bPressed = true;
		}
			break;
		case 'R':
		{
			m_tInput.bToggleRender.bPressed = true;
		}
			break;
		case '1':
		{
			m_tInput.b1.bPressed = true;
		}
			break;
		case '2':
		{
			m_tInput.b2.bPressed = true;
		}
			break;
		case '3':
		{
			m_tInput.b3.bPressed = true;
		}
			break;
		case '4':
		{
			m_tInput.b4.bPressed = true;
		}
			break;
		case 'T':
		{
			m_tInput.bReset.bPressed = true;
		}
			break;
		case VK_SHIFT:
		{
			m_tInput.bShift.bPressed = true;
		}
			break;
		case VK_OEM_3:	//TILDE
		{
			m_tInput.bTilde.bPressed = true;
		}
			break;
		default:
			break;
		}
	}
		break;
	case WM_KEYUP:
	{
		switch (LOWORD(wParam))
		{
		case VK_UP:
		{
			m_tInput.bUp.bPressed = false;
		}
			break;
		case VK_DOWN:
		{
			m_tInput.bDown.bPressed = false;
		}
			break;
		case VK_LEFT:
		{
			m_tInput.bLeft.bPressed = false;
		}
			break;
		case VK_RIGHT:
		{
			m_tInput.bRight.bPressed = false;
		}
			break;
		case 'W':
		{
			m_tInput.bW.bPressed = false;
		}
			break;
		case 'A':
		{
			m_tInput.bA.bPressed = false;
		}
			break;
		case 'S':
		{
			m_tInput.bS.bPressed = false;
		}
			break;
		case 'D':
		{
			m_tInput.bD.bPressed = false;
			break;
		}
		case 'G':
		{
			m_tInput.bG.bPressed = false;
		}
			break;
		case 'R':
		{
			m_tInput.bToggleRender.bPressed = false;
			break;
		}
		case '1':
		{
			m_tInput.b1.bPressed = false;
			break;
		}
		case '2':
		{
			m_tInput.b2.bPressed = false;
			break;
		}
		case '3':
		{
			m_tInput.b3.bPressed = false;
			break;
		}
		case '4':
		{
			m_tInput.b4.bPressed = false;
			break;
		}
		case 'T':
		{
			m_tInput.bReset.bPressed = false;
			break;
		}
		case VK_SHIFT:
		{
			m_tInput.bShift.bPressed = false;
			break;
		}
		case VK_OEM_3:	//TILDE
		{
			m_tInput.bTilde.bPressed = false;
			break;
		}
		default:
			break;
			}
		}
		break;
	}
	return DefWindowProc(m_hMainWnd, msg, wParam, lParam);
}
/**
*
* CWindow Initialises Main Window
*
* @author Christopher Howlett
*
*/
void 
CWindow::InitialiseMainWindow()
{
	printf("Setting up window at %i x %i\n", WINDOW_WIDTH, WINDOW_HEIGHT);

	wchar_t cClassName[] = L"Editor Window";
	WNDCLASS winClass;
	
	//Create window class
	winClass.cbClsExtra = 0;
	winClass.cbWndExtra = 0;
	winClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	winClass.hCursor = LoadCursor(m_hInstance, IDC_ARROW);
	winClass.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	winClass.hInstance = m_hInstance;
	winClass.lpfnWndProc = WindowProc;
	winClass.lpszClassName = cClassName;
	winClass.lpszMenuName = NULL;
	winClass.style = CS_VREDRAW | CS_HREDRAW;

	if(!RegisterClass(&winClass))
	{
		Error(L"Window creation FAILED"); 
	}

	//Initialise window
	m_hMainWnd = CreateWindow(	cClassName, 
								L"Level Editor - Christopher Howlett", 
								WS_MINIMIZEBOX | WS_VISIBLE | WS_OVERLAPPEDWINDOW,
								0,
								0,
								WINDOW_WIDTH,
								WINDOW_HEIGHT,
								NULL,
								NULL,
								m_hInstance, 
								NULL);
	if(!m_hMainWnd)
	{
		Error(L"Window creation failed2");
	}
}