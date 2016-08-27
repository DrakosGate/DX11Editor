
#pragma once

#include "defines.h"

struct TSetupStruct;
class Clock;

//Renderer interface
class IRenderer
{
public:
	IRenderer(){};
	virtual ~IRenderer(){};

	virtual bool Initialise( HWND _hWnd, TSetupStruct* _pSetupData, int _iWindowWidth, int _iWindowHeight ) = 0;
	virtual void CleanUp(){};

	virtual void ExecuteOneFrame( Clock* _pClock, float _fDeltaTick ) = 0;
	virtual void SetFPSCount( const int _fps ){}

private:
	IRenderer(const IRenderer& _kr);
	IRenderer& operator= (const IRenderer& _rhs);

protected:
};

