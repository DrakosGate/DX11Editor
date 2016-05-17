
#pragma once

#include "defines.h"

struct TSetupStruct;

//Renderer interface
class IRenderer
{
public:
	IRenderer(){};
	virtual ~IRenderer(){};

	virtual bool Initialise( HWND _hWnd, TSetupStruct* _pSetupData, int _iWindowWidth, int _iWindowHeight ) = 0;
	virtual void CleanUp(){};

private:
	IRenderer(const IRenderer& _kr);
	IRenderer& operator= (const IRenderer& _rhs);

protected:
};

