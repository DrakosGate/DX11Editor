//
// Bachelor of Software Engineering - Year 2
// Media Design School
// Auckland 
// New Zealand
//
// (c) 2013 Media Design School
//
//  File Name   :   consolewindow.cpp
//  Description :   Code for CConsoleWindow class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

// Library Includes
#include <Windows.h>
#include <iostream>

// Local Includes

// This Include
#include "consolewindow.h"

// Static Variables

// Static Function Prototypes

// Implementation
/**
*
* CConsoleWindow Constructor
*
* @author Christopher Howlett
*
*/
CConsoleWindow::CConsoleWindow()
{

}
/**
*
* CConsoleWindow Destructor
*
* @author Christopher Howlett
*
*/
CConsoleWindow::~CConsoleWindow()
{

}
/**
*
* CConsoleWindow Start up
*
* @author Christopher Howlett
*
*/
void 
CConsoleWindow::InitialiseConsole()
{
	//Initialise a seperate console window for debug output
	AllocConsole();

	FILE tIostream;
	FILE* pFile = &tIostream;
	//Enable iostream functionality
	freopen_s(&pFile, "CONIN$", "r", stdin);
	freopen_s(&pFile, "CONOUT$", "w", stdout);
}
/**
*
* CConsoleWindow Shutdown
*
* @author Christopher Howlett
*
*/
void 
CConsoleWindow::ShutdownConsole()
{
	FreeConsole();
}