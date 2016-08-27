
// WINDOWS DEFINES
#define WIN32_LEAN_AND_MEAN

// Includes
#include <Windows.h>

#include "engine.h"

int WINAPI WinMain( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _cmdLine, int _iCmdShow )
{
	Engine game_engine;
	game_engine.Initialise();

	game_engine.Run();

	return 0;
}