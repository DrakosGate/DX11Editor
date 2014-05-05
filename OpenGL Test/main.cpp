//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   main.cpp
//  Description :   Code for main
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//


// Library Includes
#include <vld.h>

// Local Includes
#include "window.h"

/**
*
* Main function
*
* @author Christopher Howlett
*
*/
int WINAPI 
WinMain( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _pCmdLine, int _iCmdShow)
{
	//Initialise window
	CWindow* pGameWindow = new CWindow();

	pGameWindow->Initialise(_hInstance, RENDERER_DIRECTX11);
	pGameWindow->Run();

	delete pGameWindow;
	pGameWindow = 0;

	return 0;
}