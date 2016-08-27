
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
* ConsoleWindow Constructor
*
* @author Christopher Howlett
*
*/
ConsoleWindow::ConsoleWindow()
{

}
/**
*
* ConsoleWindow Destructor
*
* @author Christopher Howlett
*
*/
ConsoleWindow::~ConsoleWindow()
{

}
/**
*
* ConsoleWindow Start up
*
* @author Christopher Howlett
*
*/
void
ConsoleWindow::InitialiseConsole()
{
	//Initialise a seperate console window for debug output
	AllocConsole();

	FILE tIostream;
	FILE* pFile = &tIostream;
	//Enable iostream functionality
	freopen_s( &pFile, "CONIN$", "r", stdin );
	freopen_s( &pFile, "CONOUT$", "w", stdout );
}
/**
*
* ConsoleWindow Shutdown
*
* @author Christopher Howlett
*
*/
void
ConsoleWindow::ShutdownConsole()
{
	FreeConsole();
}