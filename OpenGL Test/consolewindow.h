//
//  File Name   :   consolewindow.h
//  Description :   Declaration of CConsoleWindow class
//  Author      :   Christopher Howlett
//  Mail        :   drakos_gate@yahoo.com
//

#pragma once

#ifndef __CONSOLEWINDOW_H__
#define __CONSOLEWINDOW_H__

// Library Includes

// Local Includes

// Types

// Constants

// Prototypes

class CConsoleWindow
{
//Member functions
public:
	CConsoleWindow();
	~CConsoleWindow();

	void InitialiseConsole();
	void ShutdownConsole();
	
private:
	CConsoleWindow(const CConsoleWindow& _kr);
	CConsoleWindow& operator= (const CConsoleWindow& _rhs);

//Member variables
private:

};

#endif //__CONSOLEWINDOW_H__
