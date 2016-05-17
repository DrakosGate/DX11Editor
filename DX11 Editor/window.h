//
//  File Name   :   CWindow.h
//  Description :   Class of Window App
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __WINDOW_H__
#define __WINDOW_H__

// Library Includes
#include <Windows.h>
#include "defines.h"

// Local Includes

// Types
enum ERendererType
{
	RENDERER_INVALID = -1,
	RENDERER_OPENGL,
	RENDERER_DIRECTX11,
	RENDERER_MAX
};

// Constants

// Prototypes
class IRenderer;
class Clock;
class CConsoleWindow;

class CWindow
{
public:
	CWindow();
	virtual ~CWindow();

	virtual bool Initialise(HINSTANCE _hInstance, ERendererType _eRenderer);
	void Run();
	void ExecuteOneFrame();
	virtual void ReadProgramSetupFile(char* _pcFilename);

	HINSTANCE GetInstance();
	HWND GetWindowHandle();

	//This windows message proc
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);
	
protected:
	void InitialiseMainWindow();
private:
	CWindow(const CWindow& _kr);
	CWindow& operator= (const CWindow& _rhs);

protected:
	IRenderer* m_pRenderer;
	CConsoleWindow* m_pConsoleWindow;
	TSetupStruct* m_pSetupData;

	HINSTANCE m_hInstance;
	HWND      m_hMainWnd;
	bool      mMinimized;
	bool      mMaximized;
	bool      mResizing;

	Clock* m_pClock;

	// Derived class should set these in derived constructor to customize starting values.
	int m_iClientWidth;
	int m_iClientHeight;
};

#endif //__WINDOW_H__