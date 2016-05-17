//
//// Library Includes
//#include <math.h>
//
//// Local Includes
//
//// This Include
//#include "openglrenderer.h"
//
//// Static Variables
//
//// Static Function Prototypes
//
//// Implementation
///**
//*
//* COpenGLRenderer Constructor
//*
//* @author Christopher Howlett
//*
//*/
//COpenGLRenderer::COpenGLRenderer()
//{
//
//}
///**
//*
//* COpenGLRenderer Destructor
//*
//* @author Christopher Howlett
//*
//*/
//COpenGLRenderer::~COpenGLRenderer()
//{
//
//
//	//Shutdown OpenGL
//	CleanUp();
//}
///**
//*
//* COpenGLRenderer Initialisation
//*
//* @author Christopher Howlett
//* @param _hWnd Handle to window
//* @param _iWindowWidth Window width
//* @param _iWindowHeight Window Height
//* @param _pInput Struct used for user input
//*
//*/
//bool
//COpenGLRenderer::Initialise( HWND _hWnd, TSetupStruct* _pSetupData, int _iWindowWidth, int _iWindowHeight )
//{
//	printf( "Initialising OpenGL\n" );
//	////Initialise GLEW for additional OpenGL functionality
//	//glewInit();
//	//
//	//m_hWindow = _hWnd;
//	//m_pInput = _pInput;
//	//m_iWindowWidth = _iWindowWidth;
//	//m_iWindowHeight = _iWindowHeight;
//
//	////Start up console
//	//m_pConsole = new CConsoleWindow();
//	////m_pConsole->InitialiseConsole();
//	//
//	////Initialise OpenGL 
//	//SetupOpenGL(_hWnd);
//
//	////Load the shader files
//	//LoadShaders();
//	////Load the texture files
//	//LoadTextures();
//	////Add lights to lightmanager
//	//CreateLights();
//	////Create bitmap font
//	//SetupFont(L"Verdana");
//
//	//
//	////Create in game entities
//	//CreateEntities();
//	//
//	////Once initialisation is complete, return focus to main window
//	////m_pConsole->ShutdownConsole();
//
//	return true;
//}
///**
//*
//* COpenGLRenderer SetupOpenGL
//*
//* @author Christopher Howlett
//* @param _hWnd Handle to window used for OpenGl
//*
//*/
//void
//COpenGLRenderer::SetupOpenGL( HWND _hWnd )
//{
//	//m_hDevContext = GetDC(m_hWindow);
//	//
//	//int iPixelFormat = 0;
//	//PIXELFORMATDESCRIPTOR pFormat;
//	//ZeroMemory(&pFormat, sizeof(PIXELFORMATDESCRIPTOR));
//
//	//pFormat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
//	//pFormat.nVersion = 1;
//	//pFormat.iLayerType = PFD_MAIN_PLANE;
//	//pFormat.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
//	//pFormat.cDepthBits = 32;
//	//pFormat.cColorBits = 24;
//	//pFormat.iPixelType = PFD_TYPE_RGBA;
//	//
//	//iPixelFormat = ChoosePixelFormat(m_hDevContext, &pFormat);
//	//SetPixelFormat(m_hDevContext, iPixelFormat, &pFormat);
//	//m_hRenderContext = wglCreateContext(m_hDevContext);
//	//wglMakeCurrent(m_hDevContext, m_hRenderContext);
//
//	////Load openGl extensions
//	//LoadExtensions();
//
//	////Setup OpenGL for Rendering
//	//m_fFOV = 45.0f;
//	//m_fAspectRatio = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
//	//glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
//	//glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
//	//glEnable(GL_DEPTH_TEST);
//	//
//	//glFrontFace(GL_CW);
//	//glEnable(GL_CULL_FACE);
//	//glCullFace(GL_BACK);
//
//	////Setup matrices
//	//NMatrix::Identity(m_matWorld);	
//}